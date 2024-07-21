using UnityEngine;

public class NiseUFO_Character : CharacterActor
{
    [Header("座標の値が大きい方をend_xに設定すること")]
    [SerializeField] private float startPos_x      = 0.0f;
    [SerializeField] private float endPos_x        = 0.0f;

    private void Start()
    {
        //パラメータと各種コンポーネントを取得する
        ParameterDatas paramD = Resources.Load<ParameterDatas>("ParameterDatas");
        pData = paramD.ニセUFOステータス;
        rb    = GetComponent<Rigidbody>();

        edgePoint = transform.GetChild(0);

        //ジャンプ機能作動
        Start_JumpAtRegularInterval();

        //移動機能作動
        Start_RoundTrip_x(startPos_x, endPos_x);
    }
}