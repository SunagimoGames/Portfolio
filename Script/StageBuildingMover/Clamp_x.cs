using UnityEngine;

public class Clamp_x : MonoBehaviour
{
    [Header("ˆÚ“®”ÍˆÍ")]
    [SerializeField] private float minPos_x;
    [SerializeField] private float maxPos_x;

   private void Update()
    {
        SetLimitPos_x();
    }

    private void SetLimitPos_x()
    {
        //X‚É‚Â‚¢‚ÄˆÚ“®”ÍˆÍ‚Ì§ŒÀ‚ğs‚¤
        if(transform.position.x < minPos_x)
        {
            transform.position = new Vector3(minPos_x,
                                             transform.position.y,
                                             transform.position.z);
        }
        if(transform.position.x > maxPos_x)
        {
            transform.position = new Vector3(maxPos_x,
                                             transform.position.y,
                                             transform.position.z);
        }
    }
}
