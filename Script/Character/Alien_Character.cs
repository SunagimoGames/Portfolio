using Cysharp.Threading.Tasks;
using DG.Tweening;
using System;
using UnityEngine;

public class Alien_Character : CharacterActor
{
    [Header("敵との当たり判定に使用する足の座標オブジェクト")]
    [SerializeField] private GameObject footPoint;

    public event Action LifeDecreaseEvent = null;

    //BeInvincible()で使用
    private bool isInvincible = false;
    private readonly float flashingInterval = 0.25f;
    private readonly int invincibleSecond   = 4;

    private Caller_CamShake camShake = null;

    public new void InstantDeath()
    {
        base.InstantDeath();
        LifeDecreaseEvent?.Invoke();
    }

    private void Start()
    {
        //パラメータと各種コンポーネントを取得する
        ParameterDatas paramD = Resources.Load<ParameterDatas>("ParameterDatas");
        rb       = GetComponent<Rigidbody>();
        pData    = paramD.えいりあんステータス;
        camShake = GetComponent<Caller_CamShake>();

        //ジャンプ攻撃判定に使用するedgePointを取得
        edgePoint = transform.GetChild(0);
    }

    private new void Update()
    {
        base.Update();
        MoveByInput();
    }

    private void MoveByInput()
    {
        if (pData.移動するかどうか &&
           (Input.GetKey(KeyCode.RightArrow) || Input.GetKey(KeyCode.LeftArrow)))
        {
            //左右移動
            int direction   = (Input.GetKey(KeyCode.LeftArrow)) ? -1 : 1;
            Vector3 movePos = new(direction * pData.動く速さ, 0, 0);
            rb.velocity     = new(movePos.x, rb.velocity.y, 0);
        }

        //ジャンプ
        if (pData.ジャンプするかどうか &&
            pData.IsOnFloor            &&
            Input.GetKeyDown(KeyCode.Space)) 
        {
            Vector3 force = new(0, pData.ジャンプの速さ, 0);
            rb.AddForce(force, ForceMode.VelocityChange);
            SoundManager.instance.PlaySE("Jump");
            pData.IsOnFloor = false;
        }
    }

    private async UniTask BeInvincible()
    {
        isInvincible = true;

        //無敵中の点滅処理
        SpriteRenderer sRenderer = GetComponent<SpriteRenderer>();
        Color original  = sRenderer.color;
        Color invisible = new(255, 255, 255, 0); //半透明
        Sequence sequence = DOTween.Sequence();
        sequence.Append(sRenderer.DOColor(invisible, 0.01f)); //アルファ値を短時間で最小最大に設定することで点滅とさせる
        sequence.AppendInterval(flashingInterval);
        sequence.Append(sRenderer.DOColor(original, 0.01f));
        sequence.AppendInterval(flashingInterval);
        sequence.SetLoops(-1);
        sequence.Play();

        await UniTask.Delay(invincibleSecond * 1000);

        sequence.Kill();
        sRenderer.color = original;
        isInvincible = false;
    }   

    private void OnTriggerEnter(Collider col)
    {
        string objTag = col.gameObject.tag;
        if (objTag != "Enemy") return;

        //一定Y座標以上相手より高い場合は踏みつけ攻撃とみなし、
        //相手のライフを削って小ジャンプする
        //攻撃した,されたに限らずカメラ揺れを実行
        CharacterActor enemyActor = col.GetComponent<CharacterActor>();
        Transform enemyEdgePoint  = enemyActor.GetEdgePoint();
        if (edgePoint.position.y >= enemyEdgePoint.position.y)
        {
            enemyActor.AdjustLife(-1);
            Vector3 jumpForce = new(rb.velocity.x,
                                    pData.ジャンプの速さ,
                                    rb.velocity.z);
            rb.velocity = jumpForce;
            camShake.Call_camShake();
            return;
        }

        if (isInvincible) return;

        pData.ライフ--;
        LifeDecreaseEvent?.Invoke();
        BeInvincible().Forget();
        camShake.Call_camShake();
        return;
    }

    private void OnCollisionStay(Collision col)
    {
        string colTag = col.gameObject.tag;
        if (colTag != "Floor") return;

        pData.IsOnFloor = true;
    }
}