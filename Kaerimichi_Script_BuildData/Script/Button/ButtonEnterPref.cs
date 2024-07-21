using UnityEngine;
using UnityEngine.EventSystems;

/// <summary>
/// 直近でカーソルが一致したボタンを記録する
/// </summary>
public class ButtonEnterPref : MonoBehaviour,IPointerEnterHandler
{
    public void OnPointerEnter(PointerEventData eventData)
    {
        PlayerPrefs.SetString("RecentEnteredButtonName", gameObject.name); 
    }
}