using System.Collections.Generic;
using System.Linq;
using UnityEngine;
using UnityEngine.Audio;

public class SoundManager : MonoBehaviour
{
    public static SoundManager instance;

    [System.Serializable]
    public struct SoundData
    {
        public string name;
        public bool canOverlap;
        public AudioClip clip;
    }

    [Header("音源管理")]
    [SerializeField] private SoundData[] soundDatas_BGM = null;
    [SerializeField] private SoundData[] soundDatas_SE  = null;

    [Header("チャンネル管理")]
    [SerializeField] private int channelNum_BGM = 1;
    [SerializeField] private int channelNum_SE  = 4;
    [SerializeField] private List<AudioSource> audioSources_BGM   = null;
    [SerializeField] private List<AudioSource> audioSources_SE    = null;
    private Dictionary<string, SoundData> soundDataDictionary     = new Dictionary<string, SoundData>();

    private void Awake()
    {
        if (instance == null)
        {
            instance = this;
            DontDestroyOnLoad(this);
        }
        else Destroy(gameObject);

        //BGM,SEごとにチャンネル分、AudioSourceを持ったGameObjectを生成,リストに格納する
        //AudioSource単体ではなくそれをアタッチしたGameObjectを生成する理由は
        //同一GameObjectに同じコンポーネントを複数アタッチできないため

        for (int i = 0; i < channelNum_BGM; i++)
        {
            GameObject obj = new GameObject();
            AudioSource aSource = obj.AddComponent<AudioSource>();
            aSource.loop = true;
            obj.name = "sourceObj_BGM_" + i.ToString();
            obj.transform.parent = transform;
            audioSources_BGM.Add(obj.GetComponent<AudioSource>());
        }
        for (int i = 0; i < channelNum_SE; i++)
        {
            GameObject obj = new GameObject();
            AudioSource aSource = obj.AddComponent<AudioSource>();
            obj.name = "sourceObj_SE_" + i.ToString();
            obj.transform.parent = transform;
            audioSources_SE.Add(obj.GetComponent<AudioSource>());
        }

        //BGM,SEごとに各音源の名前とデータのペアを登録
        for (int i = 0; i < soundDatas_BGM.Length; i++)
        {
            soundDataDictionary.Add(soundDatas_BGM[i].name, soundDatas_BGM[i]);
        }
        for (int i = 0; i < soundDatas_SE.Length; i++)
        {
            soundDataDictionary.Add(soundDatas_SE[i].name, soundDatas_SE[i]);
        }
    }

    //未使用のAudioSourceを探索する
    //ない場合はnullを返す
    private AudioSource FindUnusedAudioSource_BGM() => audioSources_BGM.FirstOrDefault(aSource => aSource.isPlaying == false);
    private AudioSource FindUnusedAudioSource_SE() => audioSources_SE.FirstOrDefault(aSource => aSource.isPlaying == false);

    private SoundData? FindAudioClipByName(string name)
    {
        if (soundDataDictionary.TryGetValue(name, out SoundData value))
        {
            return value;
        }
        else
        {
            Debug.LogWarning("指定音源" + name + "は存在しません");
            return null;
        }
    }

    public void PlayBGM(string name)
    {
        //音源検索
        SoundData? sData = (SoundData)FindAudioClipByName(name);
        if (sData == null) return;

        //空きのAudioSourceを確保(空きがなければチャンネル0を使用)
        AudioSource aSource = FindUnusedAudioSource_BGM();
        if (aSource == null) aSource = audioSources_BGM[0];

        aSource.clip = sData.Value.clip;
        aSource.Play();
    }

    public void PlaySE(string name)
    {
        //音源検索
        SoundData? sData = (SoundData)FindAudioClipByName(name);
        if (sData == null) return;

        //空きのAudioSourceを確保し再生(空きがなければSE鳴らさない)
        //重複再生を許すSEの場合はチャンネル0のものを確保
        AudioSource aSource = null;
        if(sData.Value.canOverlap)
        {
            aSource = audioSources_SE[0];
            aSource.PlayOneShot(sData.Value.clip);
        }
        else
        {
            aSource = FindUnusedAudioSource_SE();
            if (aSource == null) return;

            aSource.clip = sData.Value.clip;
            aSource.Play();
        }
    }
}