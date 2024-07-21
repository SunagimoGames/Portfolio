using System;
using UnityEngine;
using UnityEngine.EventSystems;

public class Window_background : MonoBehaviour,IPointerClickHandler
{
    public static event Action OnBackgroundClicked;

    public void OnPointerClick(PointerEventData eventData)
    {
        OnBackgroundClicked?.Invoke();
    }
}
