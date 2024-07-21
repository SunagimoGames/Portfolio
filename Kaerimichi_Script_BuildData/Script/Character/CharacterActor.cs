using Cysharp.Threading.Tasks;
using System.Threading;
using UnityEngine;

public class CharacterActor : MonoBehaviour
{
    public ParameterData pData;
    public int GetLife() { return pData.ライフ; }
    public void AdjustLife(int amount) { pData.ライフ += amount; }

    public void InstantDeath()
    {
        pData.ライフ = 0;
    }

    //移動範囲の制限範囲(ステージの左右端の座標に合わせる)
    private readonly float minPos_x = -9.5f;
    private readonly float maxPos_x = 9.5f;

    //自機はfootPoint,敵はheadPointをセットする
    //自機の踏みつけ攻撃判定に使用
    protected Transform edgePoint;
    public Transform GetEdgePoint() { return edgePoint; }

    protected Rigidbody rb;

    //JumpAtRegularInterval()で使用
    private float timeFromLanding = 0.0f;

    protected void Update()
    {
        SetLimitPos_x();
    }

    private void SetLimitPos_x()
    {
        //Xについて移動範囲の制限を行う
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

    protected void Start_RoundTrip_x(float startPos_x, float endPos_x)
    {
        if (pData.移動するかどうか)
        {
            CancellationToken token = this.GetCancellationTokenOnDestroy();
            RoundTrip_x(startPos_x, endPos_x, token).Forget();
        }
    }
    private async UniTask RoundTrip_x(float startPos_x, float endPos_x, CancellationToken token)
    {
        bool isMovingToEndPos = true;
        while (true)
        {
            if(isMovingToEndPos)
            {
                transform.Translate(pData.動く速さ * Time.deltaTime, 0, 0);
            }
            else
            {
                transform.Translate(-pData.動く速さ * Time.deltaTime, 0, 0);
            }
            Debug.Log(isMovingToEndPos);

            //始点または終点に到達した際、移動方向を反転させる
            if (transform.position.x > endPos_x)
            {
                isMovingToEndPos = false;
                transform.position = new(endPos_x, transform.position.y, transform.position.z);
            }
            else if (transform.position.x < startPos_x)
            {
                isMovingToEndPos = true;
                transform.position = new(startPos_x, transform.position.y, transform.position.z);
            }

            await UniTask.Yield(token);
        }
    }

    protected void Start_JumpAtRegularInterval() 
    {
        if(pData.ジャンプするかどうか && gameObject.CompareTag("Enemy"))
        {
            CancellationToken token = this.GetCancellationTokenOnDestroy();
            JumpAtRegularInterval(token).Forget();
        }
    }
    private async UniTask JumpAtRegularInterval(CancellationToken token)
    {
        //ジャンプの秒数間隔分、時間が経過した際にジャンプを行う
        while(true)
        {
            timeFromLanding += Time.deltaTime;

            if (timeFromLanding > pData.敵専用_ジャンプの秒数間隔 &&
                pData.IsOnFloor)
            {
                //ジャンプの速さの値をそのまま使用すると速すぎるのでdelayRatioを掛け算
                float delayRatio = 0.25f;
                Vector3 force    = new(0, pData.ジャンプの速さ * delayRatio, 0);
                rb.AddForce(force, ForceMode.VelocityChange);

                timeFromLanding = 0.0f;
            }

            await UniTask.Yield(token);
        }
    }

    protected void OnCollisionEnter(Collision col)
    {
        //ジャンプ中にジャンプの秒数間隔分の時間が経過して
        //着地の瞬間にまたジャンプすることを防ぐため
        //着地時にtimeFromLandingをリセットする
        string colTag = col.gameObject.tag;
        if (colTag != "Floor") return;

        timeFromLanding = 0.0f;
        pData.IsOnFloor = true;
    }

    protected void OnCollisionExit(Collision col)
    {
        string colTag = col.gameObject.tag;
        if (colTag != "Floor") return;

        pData.IsOnFloor = false;
    }
}