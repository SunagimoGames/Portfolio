using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class StageImgChanger : MonoBehaviour
{
    [SerializeField] private List<GameObject> buttonObjs = new();
    [SerializeField] private List<Sprite> sprites = new();
    [SerializeField] private Image panelImg = null;

    private void Start()
    {
        PlayerPrefs.SetString("RecentEnteredButtonName", "None"); 
    }

    private void Update()
    {
        string enteredButtonName = PlayerPrefs.GetString("RecentEnteredButtonName");
        if (enteredButtonName == "None")
        {
            panelImg.sprite = sprites[0];
        }
        for (int i = 0; i < buttonObjs.Count; i++)
        {
            if(enteredButtonName == buttonObjs[i].name)
            {
                panelImg.sprite = sprites[i];
            }
        }
    }
}