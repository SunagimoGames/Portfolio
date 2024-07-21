using UnityEngine;

public class Goal : MonoBehaviour
{
    private void OnCollisionEnter(Collision col)
    {
        if (col.gameObject.tag != "Player" ||
            Stage_director.instance.IsKeyGotton == false)
        {
            return;
        }

        SoundManager.instance.PlaySE("Goal");
        Stage_director.instance.Anim.SetBool("IsCleared", true);
    }
}