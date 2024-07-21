using DG.Tweening;
using UnityEngine;

/// <summary>
/// éwíËï˚å¸Ç≈ÇÃâùïúà⁄ìÆÇåJÇËï‘Ç∑ã@î\ÇéùÇ¬
/// </summary>
public class RoundTrip_building : MonoBehaviour
{
    private enum Direction
    {
        x,
        y,
        z
    }
    [SerializeField] private Direction direction = new();
    [SerializeField] private float moveduration  = 1.0f;
    [SerializeField] private float startPos      = 0.0f;
    [SerializeField] private float endPos        = 0.0f;

    private void Start()
    {
        Sequence seq = DOTween.Sequence();
        switch(direction)
        {
            case Direction.x:
                transform.position = new(startPos, 
                                         transform.position.y,
                                         transform.position.z);
                seq.Append(transform.DOMoveX(endPos, moveduration));
                seq.Append(transform.DOMoveX(startPos, moveduration));
                break;

            case Direction.y:
                transform.position = new(transform.position.x, 
                                         startPos,
                                         transform.position.z);
                seq.Append(transform.DOMoveY(endPos, moveduration));
                seq.Append(transform.DOMoveY(startPos, moveduration));
                break;

            case Direction.z:
                transform.position = new(transform.position.x, 
                                         transform.position.y,
                                         startPos);
                seq.Append(transform.DOMoveZ(endPos, moveduration));
                seq.Append(transform.DOMoveZ(startPos, moveduration));
                break;
        }
        seq.SetLoops(-1);
        seq.Play();
    }
}