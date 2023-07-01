using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SFXManager : MonoBehaviour //this would be a singleton
{
    [SerializeField]
    private AudioClip[] m_Clips;

    [SerializeField]
    private AudioSource m_AudioSource;

    private Dictionary<string, AudioClip> m_EffectDict;

    private static SFXManager _instance;

    private void Awake()
    {
        if (_instance == null)
        {
            _instance = this;
            DontDestroyOnLoad(_instance);
        }
        else
        {
            Destroy(this);
        }
        //create dictionary
        m_EffectDict = new Dictionary<string, AudioClip>();
        foreach(var clip in m_Clips)
        {
            Debug.Log(clip.name);
            m_EffectDict[clip.name] = clip;
        }
    }

    public static void PlayClip(string clipName)
    {
        if (_instance.m_EffectDict.ContainsKey(clipName) == false)  //check if a clip exists
        {
            Debug.Log("Clip does not exist");
            return;
        }

        AudioClip clip = _instance.m_EffectDict[clipName];

        if(clip == null) //ensure clip is not null
        {
            Debug.Log("Clip is null");
            return;
        }

        _instance.m_AudioSource.PlayOneShot(clip);
        Debug.Log("Clip played");
    }

    public static void PlayRun()
    {
        PlayClip("running");
    }
}
