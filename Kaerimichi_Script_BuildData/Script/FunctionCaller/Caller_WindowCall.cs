using UnityEngine;

public class Caller_WindowCall : MonoBehaviour
{
    [Header("呼び出すWindow")]
    [SerializeField] private Window_director windowDirector;

/***************************************************************************
外部呼出し専用関数
***************************************************************************/

    public void Call_windowCall()
    {
        windowDirector.StartWindow();
    }
}