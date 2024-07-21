using UnityEngine;

/// <summary>
/// 上からキャラを押しつぶす天井ギミック<para></para>
/// このクラスを持ったオブジェクトがキャラクターに触れたとき
/// </summary>
public class PressCeiling : MonoBehaviour
{
    private void OnCollisionStay(Collision col)
    {
        //CharacterActor継承クラスに触れたとき、そのオブジェクトが
        //地面に接していれば押しつぶしたとみなし、キャラクターを即死させる
        if (col.gameObject.TryGetComponent(out CharacterActor cActor) == false ||
            cActor.pData.IsOnFloor == false)
        {
            return;
        }

        //即死処理
        if (col.gameObject.TryGetComponent(out Alien_Character alienActor))
        {
            alienActor.InstantDeath();
        }
        else
        {
            cActor.InstantDeath();
        }
    }
}