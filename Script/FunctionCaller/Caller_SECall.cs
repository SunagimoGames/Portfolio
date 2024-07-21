using UnityEngine;

public class Caller_SECall : MonoBehaviour
{
    [SerializeField] private string SEName;

/***************************************************************************
ŠO•”ŒÄo‚µê—pŠÖ”
***************************************************************************/

    public void Call_SECall()
    {
        SoundManager.instance.PlaySE(SEName);
    }
}