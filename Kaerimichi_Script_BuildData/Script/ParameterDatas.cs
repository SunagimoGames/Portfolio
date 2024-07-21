using UnityEngine;

/// <summary>
/// 本ゲームでは全てのキャラクターパラメータをここから参照する<para></para>
/// また、イベント参加者がパラメータを把握しやすくするために変数を日本語で命名する
/// </summary>
[CreateAssetMenu(fileName = "ParameterDatas", menuName = "ParameterDatas")]
public class ParameterDatas : ScriptableObject
{
    [Header("えいりあんちゃんのステータス")]
    public ParameterData えいりあんステータス;

    [Header("UFOのステータス")]
    public ParameterData UFOステータス;

    [Header("ニセUFOのステータス")]
    public ParameterData ニセUFOステータス;
}

[System.Serializable]
public struct ParameterData
{
    [Range(0, 5)] public int ライフ;
    public bool 移動するかどうか;
    [Range(1, 100)] public float 動く速さ;
    public bool ジャンプするかどうか;
    [Range(1, 100)] public float ジャンプの速さ;
    [Range(1, 20)]  public float 敵専用_ジャンプの秒数間隔;

    [HideInInspector] public bool IsOnFloor;
}