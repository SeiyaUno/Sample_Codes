using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CannonSpawner : MonoBehaviour
{
    public Transform launchPoint;
    public GameObject projectilePrefab;

    [SerializeField, Range(0f, 360f)]
    private float rotation = 180f;

    [SerializeField, Range(0f, 20f)]
    private float bulletSpeed = 20f;


    public float shootTime = 0.5f;
    public float timeForDeath = 2.0f;

    private float timer;
    private Projectile projectile;


    // Start is called before the first frame update
    void Start()
    {
        projectile = projectilePrefab.GetComponent<Projectile>();

        timer = shootTime;
        Shoot();
    }

    // Update is called once per frame
    void Update()
    {
        //if (Input.GetKeyDown("space"))
        //{
        //    Shoot();
        //}

        projectile.SetSpeed(bulletSpeed);

        if (timer > 0)
        {
            timer -= Time.deltaTime;
        }
        else
        {
            StartCoroutine(Shoot());
            timer = shootTime;
        }
    }

    private IEnumerator Shoot()
    {
        Vector3 rot = launchPoint.rotation.eulerAngles;
        rot = new Vector3(rot.x, rot.y, rot.z + rotation);

        //GameObject bullet = Instantiate(projectilePrefab, launchPoint.position, launchPoint.rotation);
        GameObject bullet = Instantiate(projectilePrefab, launchPoint.position, Quaternion.Euler(rot));

        yield return new WaitForSeconds(timeForDeath);

        //Destroy(bullet);
        Destroy(bullet);
    }
}
