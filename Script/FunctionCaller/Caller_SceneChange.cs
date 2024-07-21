using Cysharp.Threading.Tasks;
using UnityEngine;

public class Caller_SceneChange : MonoBehaviour
{
    [SerializeField] private SceneChanger.Scenes goalScene;

/***************************************************************************
�O���ďo����p�֐�
***************************************************************************/

    public void Call_sceneChange()
    {
        UniTask.Void(async () =>
        {
            Fader.instance.FadeOut();
            await UniTask.Delay((int)(Fader.instance.GetDuration_fadeOut() * 1000));
            SceneChanger.SceneLoad(goalScene);
        });
    }
}