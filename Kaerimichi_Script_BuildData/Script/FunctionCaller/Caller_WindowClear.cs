using UnityEngine;

public class Caller_WindowClear : MonoBehaviour
{
    [Header("終了させるWindow")]
    [SerializeField] private Window_director windowDirector;

/***************************************************************************
外部呼出し専用関数
***************************************************************************/

    public void Call_windowClear()
    {
        windowDirector.EndWindow();
    }
}