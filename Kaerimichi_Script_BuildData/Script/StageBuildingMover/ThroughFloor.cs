using UnityEngine;

/// <summary>
/// 下からすり抜けられる床のクラス
/// </summary>
public class ThroughFloor : MonoBehaviour
{
    [Header("床としての判定を持つコライダー")]
    [SerializeField] private Collider coll = null;

    //トリガーに入ったものは出るまで
    //通常コライダーと衝突しないようにする
    private void OnTriggerEnter(Collider col)
    {
        Physics.IgnoreCollision(coll, col, true);
    }
    private void OnTriggerExit(Collider col)
    {
        Physics.IgnoreCollision(coll, col, false);
    }
}