using DG.Tweening;
using UnityEngine;

/// <summary>
/// w’è•ûŒü‚Å‚Ì‰ñ“]‚ğŒJ‚è•Ô‚·‹@”\‚ğ‚Â
/// </summary>
public class Rotate_building : MonoBehaviour
{
    private enum Direction
    {
        x,
        y,
        z
    }
    [SerializeField] private Direction direction = new();
    [SerializeField] private float moveduration  = 1.0f;

    private void Start()
    {
        Sequence seq = DOTween.Sequence();
        Vector3 rotateVec = Vector3.zero;
        switch(direction)
        {
            case Direction.x:
                rotateVec = new(360, 0, 0);
                break;

            case Direction.y:
                rotateVec = new(0, 360, 0);
                break;

            case Direction.z:
                rotateVec = new(0, 0, 360);
                break;
        }

        seq.Append(transform.DOLocalRotate(rotateVec, moveduration, RotateMode.FastBeyond360));
        seq.SetLoops(-1);
        seq.Play();
    }
}