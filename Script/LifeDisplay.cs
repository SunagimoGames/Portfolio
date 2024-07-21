using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

/// <summary>
/// Alienのライフに応じたハート画像表示を行う
/// </summary>
public class LifeDisplay : MonoBehaviour
{
    [SerializeField] private Alien_Character alienScript = null;
    [SerializeField] private Sprite heartImg_full         = null;
    [SerializeField] private Sprite heartImg_empty        = null;
    [SerializeField] private Vector3 heartImgOriginPos   = Vector3.zero;
    [SerializeField] private float heartImgOffset        = 0.0f;
    private readonly List<Image> imgList = new();
    private int lifeMax = 0;

    private void Start()
    {
        //Alienのライフ数を取得
        ParameterData alienParamD = Resources.Load<ParameterDatas>("ParameterDatas").えいりあんステータス;
        lifeMax = alienParamD.ライフ;

        //Imageコンポーネントを持った子オブジェクトをライフ分生成
        for (int i = 0; i < lifeMax; i++)
        {
            GameObject imgObj   = new();
            Image imgComponent  = imgObj.AddComponent<Image>();
            imgComponent.sprite = heartImg_full;
            imgObj.transform.SetParent(transform, false);

            //位置を調整して横並びに表示
            Vector3 offset = Vector3.right * heartImgOffset;
            imgObj.transform.position = heartImgOriginPos + offset * i;

            imgList.Add(imgComponent);
        }

        //ライフ減少に合わせて画像表示変更を行うイベントハンドラー登録
        alienScript.LifeDecreaseEvent += ReflectLifeNumToImgs;
    }

    /// <summary>
    /// Alienのライフ減少イベントのハンドラーに使用する
    /// </summary>
    private void ReflectLifeNumToImgs()
    {
        int lifeNum = alienScript.GetLife();

        //減少ライフ,残りライフに応じた画像に差し替え
        //残りライフ画像表示
        for (int i = 0; i < lifeMax; i++)
        {
            if(i < lifeNum)
            {
                imgList[i].sprite = heartImg_full;
            }
            else
            {
                imgList[i].sprite = heartImg_empty;
            }
        }
    }
}