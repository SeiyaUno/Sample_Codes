using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class PauseMenu : MonoBehaviour
{
    public GameObject PausePanel;
    public bool GamePaused;
    // Start is called before the first frame update
    void Start()
    {
        PausePanel.SetActive(false);
    }

    // Update is called once per frame
    void Update()
    {
        if(Input.GetKeyDown(KeyCode.Escape) && PausePanel.activeSelf == false)
        {
            PauseGame();
        }
    }

    public void PauseGame()
    {
        Debug.Log("PAUSE");
        PausePanel.SetActive(true);
        Time.timeScale = 0f;
        GamePaused = true;
    }

    public void ResumeGame()
    {
        Debug.Log("RESUME");
        Time.timeScale = 1f;
        GamePaused = false;
        PausePanel.SetActive(false);
    }

    public void RestartGame()
    {
        Debug.Log("RESTART");
        Time.timeScale = 1f;
        Scene cur_scene = SceneManager.GetActiveScene();
        SceneManager.LoadScene(cur_scene.name);
        PausePanel.SetActive(false);
    }

    public void BackToMainMenu()
    {
        Debug.Log("BACKTOMAIN");
        Time.timeScale = 1f;
        SceneManager.LoadScene(0);
        PausePanel.SetActive(false);
    }


    

}
