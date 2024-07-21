using Cysharp.Threading.Tasks;
using UnityEngine;
using UnityEngine.SceneManagement;

/// <summary>
/// ステージのカギの取得状態,ゴールしたかどうかによる演出を司る
/// </summary>
public class Stage_director : MonoBehaviour
{
    public static Stage_director instance = null;

    public bool IsKeyGotton { get; set; } = false;

    public Animator Anim { get; private set; } = null;

    //ゲームオーバー時のwindow表示に使用
    [SerializeField] private GameObject alienObj;
    [SerializeField] private Caller_WindowCall winCaller;

    [Header("Stage_03の生成情報")]
    [SerializeField] private Vector3 generatePos;
    [SerializeField] private GameObject UFO;
    [SerializeField] private GameObject niseUFO;
    [SerializeField] private GameObject key;
    [SerializeField] private GameObject goal;

    private void Start()
    {
        instance = this;
        Anim = GetComponent<Animator>();

        //ステージごとのBGM再生
        string sceneName = SceneManager.GetActiveScene().name;
        switch (sceneName)
        {
            case "Stage_00":
                SoundManager.instance.PlayBGM("Stage_00");
                break;

            case "Stage_01":
                SoundManager.instance.PlayBGM("Stage_01");
                break;

            case "Stage_02":
                SoundManager.instance.PlayBGM("Stage_02");
                break;

            case "Stage_03":
                SoundManager.instance.PlayBGM("Stage_03");
                break;
        }

        if (sceneName == "Stage_03")
        {
            ExecuteEnemyGenerate();
        }
    }

    private void Update()
    {
        if (alienObj == null)
        {
            winCaller.Call_windowCall();
        }
    }

    private void ExecuteEnemyGenerate()
    {
        UniTask.Void(async () =>
        {
            Instantiate(UFO, generatePos, Quaternion.identity);
            await UniTask.Delay(5000);
            Instantiate(UFO, generatePos, Quaternion.identity);
            await UniTask.Delay(3000);
            Instantiate(UFO, generatePos, Quaternion.identity);
            await UniTask.Delay(1500);
            Instantiate(niseUFO, generatePos, Quaternion.identity);
            await UniTask.Delay(1000);

            Instantiate(UFO, generatePos, Quaternion.identity);
            await UniTask.Delay(500);

            Instantiate(UFO, generatePos, Quaternion.identity);
            await UniTask.Delay(500);

            Instantiate(UFO, generatePos, Quaternion.identity);
            await UniTask.Delay(1500);

            Instantiate(UFO, generatePos, Quaternion.identity);
            await UniTask.Delay(1000);

            Vector3 keyRot = new(0, 0, -90);
            GameObject keyObj = Instantiate(key, generatePos, Quaternion.Euler(keyRot));
            keyObj.transform.localScale = new(10, 10, 10);
            await UniTask.Delay(1000);

            Instantiate(goal, generatePos, Quaternion.identity);
            await UniTask.Delay(1000);
        });
    }
}