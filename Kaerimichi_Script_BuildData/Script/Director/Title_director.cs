using UnityEngine;

public class Title_director : MonoBehaviour
{
    private bool canSceneMoveByPushSpace = false;
    private Animator anim;

    private void Start()
    {
        anim = GetComponent<Animator>();
        SoundManager.instance.PlayBGM("Title");
    }

    private void Update()
    {
        if (canSceneMoveByPushSpace)
        {
            if (Input.GetKeyUp(KeyCode.Space))
            {
                SoundManager.instance.PlaySE("TitleButton");
                anim.SetBool("isSpaceKeyPushed", true);
            }
        }
    }

/********************************************************************
アニメーションイベント用関数
*******************************************************************/

    public void EnablePushingSpace()
    {
        canSceneMoveByPushSpace = true;
    }
}