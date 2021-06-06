using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class GhostMove : MonoBehaviour
{
    public GameObject[] wayPointsGos;
    public float speed = 0.2f;//鬼的移动速度为0.2个单位
    private List<Vector3> wayPoints = new List<Vector3>();
    private int index = 0;//当前走过的距离
    private Vector3 startPos;
//开始设置函数
    private void Start()
    {
        startPos = transform.position + new Vector3(0, 3, 0);//开始的位置在放置位置上方的三个单位
        LoadAPath(wayPointsGos[GameManager.Instance.usingIndex[GetComponent<SpriteRenderer>().sortingOrder - 2]]);//加载一条道路
    }
//更新函数
    private void FixedUpdate()
    {
        if (transform.position != wayPoints[index])//当前的位置不是道路的尽头
        {
            Vector2 temp = Vector2.MoveTowards(transform.position, wayPoints[index], speed);//插值得到要移动到wayPoints[index]位置的下一次移动坐标
            GetComponent<Rigidbody2D>().MovePosition(temp);//通过刚体来设置物体的位置
        }
        else
        {
            index++;//距离自增
            if (index >= wayPoints.Count)//走过的距离超过道路的总长度
            {
                index = 0;//走过的距离初始化为0
                LoadAPath(wayPointsGos[Random.Range(0, wayPointsGos.Length)]);//重新加载一条道路
            }
        }
        Vector2 dir = wayPoints[index] - transform.position;//获取移动方向
        GetComponent<Animator>().SetFloat("DirX", dir.x);//把获取到的移动方向设置给动画状态机
        GetComponent<Animator>().SetFloat("DirY", dir.y);
    }
//加载道路
    private void LoadAPath(GameObject go)
    {
        wayPoints.Clear();//道路清空
        foreach (Transform t in go.transform)
        {
            wayPoints.Add(t.position);
        }
        wayPoints.Insert(0, startPos);//插入起始点的位置
        wayPoints.Add(startPos);//增加起始点的位置
    }
//发生人物的碰撞
    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.gameObject.name == "Pacman")//对象是吃豆人
        {
            if (GameManager.Instance.isSuperPacman)//当前是超级吃豆人
            {
                transform.position = startPos - new Vector3(0, 3, 0);//将鬼送至初始位置
                index = 0;//道路距离设置为0
                GameManager.Instance.score += 500;//当前得分加500
            }
            else
            {
                collision.gameObject.SetActive(false);//碰撞体消失
                GameManager.Instance.gamePanel.SetActive(false);
                Instantiate(GameManager.Instance.gameoverPrefab);//调用游戏结束对象显示游戏结束
                Invoke("ReStart", 3f);//游戏结束后三秒重新加载初始场景
            }
        }
    }
//重新开始函数
    private void ReStart()
    {
        SceneManager.LoadScene(0);//加载初始场景
    }
}
