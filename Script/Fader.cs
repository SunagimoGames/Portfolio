using DG.Tweening;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;
using System;

/// <summary>
/// シーン読み込み時に明転(フェードイン)処理を行う<para></para>
/// シーンをまたいでも破棄されない
/// </summary>
public class Fader : MonoBehaviour
{
    public static Fader instance;

    [Header("フェードイン完了までの時間設定")]
    [SerializeField] private float duration_fadeIn = 0.25f;
    public float GetDuration_fadeIn() { return duration_fadeIn; }

    [Header("フェードアウト完了までの時間設定")]
    [SerializeField] private float duration_fadeOut = 0.25f;
    public float GetDuration_fadeOut() { return duration_fadeOut; }

    private Image fadeImg;

    private void Awake()
    {
        if(instance == null)
        {
            instance = this;
            fadeImg  = GetComponent<Image>();
            DontDestroyOnLoad(transform.parent);
            SceneManager.sceneLoaded += FadeInAfterSceneLoaded;
        }
        else
        {
            Destroy(gameObject);
        }
    }

    private void FadeInAfterSceneLoaded(Scene nextScene, LoadSceneMode mode) 
    {
        //一瞬で黒背景を可視化した後に
        //フェードインを行う
        Sequence sequence = DOTween.Sequence();
        sequence.Append(fadeImg.DOFade(1.0f, 0.0f));
        sequence.Append(fadeImg.DOFade(0.0f, duration_fadeIn));
	}

    public void FadeOut()
    {
        fadeImg.DOFade(1.0f, duration_fadeOut);
    }
}