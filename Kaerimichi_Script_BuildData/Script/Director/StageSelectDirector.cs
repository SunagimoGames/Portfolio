using UnityEngine;

public class StageSelectDirector : MonoBehaviour
{
    private void Start()
    {
        SoundManager.instance.PlayBGM("StageSelect");
    }
}