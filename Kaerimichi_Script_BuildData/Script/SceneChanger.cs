using UnityEngine;
using UnityEngine.SceneManagement;

public class SceneChanger
{
    //Scenes各要素に対応するシーンは
    //その名前もScenes要素と一致させること
    public enum Scenes
    {
        Title,
        StageSelect,
        Stage_00,
        Stage_01,
        Stage_02,
        Stage_03,
        Stage_04
    }

    public static void SceneLoad(Scenes scene)
    {
        Debug.Log(scene);
        string sceneStr = scene.ToString();
        if(FindSameScenes(sceneStr))
        {
            SceneManager.LoadScene(sceneStr);
        }
        else
        {
            Debug.LogError($"シーン '{sceneStr}' は存在しない");
        }
    }

    //引数と一致する名前のシーンがあるかで真偽値を返す
    private static bool FindSameScenes(string sceneName)
    {
        for (int i = 0; i < SceneManager.sceneCountInBuildSettings; i++)
        {
            string scenePath = SceneUtility.GetScenePathByBuildIndex(i);
            string sceneNameFromPath = System.IO.Path.GetFileNameWithoutExtension(scenePath);
            if (sceneNameFromPath == sceneName)
            {
                return true;
            }
        }
        return false;
    }
}