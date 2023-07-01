using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class MainMenu : MonoBehaviour
{


    public GameObject Cp;
    // Start is called before the first frame update
    void Start()
    {
        //VP.SetActive(false);
        Cp.SetActive(false);
        PlayerPrefs.SetString("weapon1", "null");
        PlayerPrefs.SetString("weapon2", "null");
        PlayerPrefs.SetString("weapon3", "null");
    }

    // Update is called once per frame
    void Update()
    {

    }

    public void StartGame()
    {
        Debug.Log("START");
        SceneManager.LoadScene(1);
    }


    public void QuitGame()
    {
        Debug.Log("QUIT");
        Application.Quit();
    }

    public void OpenControlPanel()
    {
        Cp.SetActive(true);
    }

    public void CloseControlPanel()
    {
        Cp.SetActive(false);
    }

}
