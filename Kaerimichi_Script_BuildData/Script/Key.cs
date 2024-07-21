using UnityEngine;

public class Key : MonoBehaviour
{
    private void OnCollisionEnter(Collision col)
    {
        if (col.gameObject.CompareTag("Player") == false) return;

        Stage_director.instance.IsKeyGotton = true;
        SoundManager.instance.PlaySE("Key");
        Destroy(gameObject);
    }
}