using DG.Tweening;
using UnityEngine;

/// <summary>
/// カメラ揺れ機能を提供する
/// </summary>
public class Caller_CamShake : MonoBehaviour
{
    [Header("カメラ未指定でMainCameraを使用")]
    [SerializeField] private Camera cam      = null;
    [SerializeField] private float  strength = 1.0f;
    [SerializeField] private float  duration = 0.15f;

    public void SetShakeTargetCam(Camera c) { cam = c; }
    public void SetStrength(float stren) { strength = stren; }
    public void SetDuration(float dura) { duration = dura; }

/***************************************************************************
外部呼出し専用関数
***************************************************************************/
    public void Call_camShake()
    {
        if(cam == null)
        {
            cam = Camera.main;
        }
        cam.transform.DOShakePosition(duration, strength);
    }
}