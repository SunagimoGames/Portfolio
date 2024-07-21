using UnityEngine;

/// <summary>
/// CharacterActor継承クラスのライフがゼロ以下で
/// 指定パーティクルを生成してGameObjectを破棄する
/// </summary>
public class DeadParticle : MonoBehaviour
{
    [Header("やられ時のパーティクル")]
    [SerializeField] private ParticleSystem particle;
    private CharacterActor actor;

    private void Start()
    {
        actor = GetComponent<CharacterActor>();
    }

    private void Update()
    {
        if(actor.GetLife() <= 0)
        {
            SoundManager.instance.PlaySE("Dead");
            Instantiate(particle, transform.position, Quaternion.identity);
            Destroy(gameObject);
        }
    }
}