using UnityEngine;
using UnityEngine.UI;
using DG.Tweening;

public class Window_director : MonoBehaviour
{
    [SerializeField] private Image background;
    [SerializeField] private RectTransform windowRectT;

    [Header("暗転についての設定")]
    [SerializeField] private float maxBackgroundAlpha = 0.75f;
    [SerializeField] private float duration_blackout  = 0.25f;

    [Header("ウィンドウについての設定")]
    [SerializeField] private bool canAppearAsStartup = false;
    [SerializeField][Range(0, 1)] private float startScale = 0.0f;
    [SerializeField][Range(0, 1)] private float maxScale   = 1.0f;
    [SerializeField] private float duration_winAppear      = 1.0f;

    private void Start()
    {
        if (canAppearAsStartup)
        {
            StartWindow();
        }
        else
        {
            EndWindow();
        }
    }

    private void OnEnable()
    {
        windowRectT.localScale = new Vector3(startScale, startScale, startScale);
        Window_background.OnBackgroundClicked += HandleBackgroundClicked;
    }

    private void OnDisable()
    {
        Window_background.OnBackgroundClicked -= HandleBackgroundClicked;
    }

    private void HandleBackgroundClicked()
    {
        Debug.Log("Windowを無効化");
        EndWindow();
    }

    //これで外部からWindowを始動させる
    public void StartWindow()
    {
        gameObject.SetActive(true); 
        Animator anim = GetComponent<Animator>();
        anim.SetBool("canAppear", true);
    }

    //これで外部からWindowを終了させる
    //アニメーションイベントと黒背景クリック時に使用
    public void EndWindow()
    {
        Animator anim = GetComponent<Animator>();
        anim.SetBool("canAppear", false);
        gameObject.SetActive(false); 
    }

/***************************************************************************
アニメーションイベント用関数
***************************************************************************/

    public void BlockoutBackground_animEvent()
    {
        background.DOFade(maxBackgroundAlpha, duration_blackout);
    }

    public void AppearWindows_animEvent()
    {
        windowRectT.DOScale(maxScale, duration_winAppear).SetEase(Ease.OutBounce);
    }
}