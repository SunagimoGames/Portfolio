using DG.Tweening;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;

public class ButtonEffectManager : MonoBehaviour,IPointerEnterHandler,IPointerExitHandler,IPointerClickHandler
{
    [Header("カーソル一致,不一致時に表示するスプライト")]
    [SerializeField] private Sprite sprite_enter;
    [SerializeField] private Sprite sprite_exit;
    [Header("クリック時の強調表現について")]
    [SerializeField] private float zoomInScale           = 1.05f;
    [SerializeField] private float zoomInDuration        = 0.1f;
    [SerializeField] private float beOriginScaleDuration = 0.2f;
    private Vector3 originScale = Vector3.one;
    private Image img = null;

    private void Start()
    {
        originScale = transform.localScale;
        img         = GetComponent<Image>();
    }

    //カーソル一致時の拡大演出
    public void OnPointerEnter(PointerEventData eventData)
    {
        if (sprite_enter == null) return;
        img.sprite = sprite_enter;
    }
    public void OnPointerExit(PointerEventData eventData)
    {
        if (sprite_exit == null) return;
        img.sprite = sprite_exit;
    }

    //クリック時の縮小演出
    public void OnPointerClick(PointerEventData eventData)
    {
        Sequence sequence = DOTween.Sequence()
            .Append(transform.DOScale(originScale * zoomInScale, zoomInDuration))
            .OnComplete(() => transform.DOScale(originScale, beOriginScaleDuration));
    }
}