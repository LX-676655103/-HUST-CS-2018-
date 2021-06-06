using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
//导入unity相关的命名空间
public class GameManager : MonoBehaviour
{
    private static GameManager _instance;//实例化对象声明
    public static GameManager Instance
    {
        get
        {
            return _instance;//返回单例变量
        }
    }
   
    public GameObject pacman;//对象吃豆人
    public GameObject blinky;//对象鬼
    public GameObject clyde;//对象鬼
    public GameObject inky;//对象鬼
    public GameObject pinky;//对象鬼
    public GameObject startPanel;//start按键对象
    public GameObject gamePanel;//游戏对象
    public GameObject startCountDownPrefab;//倒计时显示对象
    public GameObject gameoverPrefab;//游戏结束对象
    public GameObject winPrefab;//游戏胜利对象
    public AudioClip startClip;//点击开始对象
    public Text remainText;//剩余豆子数量
    public Text nowText;//当前吃掉的豆子
    public Text scoreText;//得分
    public Button PlayBtn;//声音开按键对象
    public Button StopBtn;//声音关按键对象

    public bool isSuperPacman = false;//是否属于超级吃豆人状态
    public List<int> usingIndex = new List<int>();
    public List<int> rawIndex = new List<int> { 0, 1, 2, 3 };
    private List<GameObject> pacdotGos = new List<GameObject>();
    private int pacdotNum = 0;//豆子总数
    private int nowEat = 0;//当前吃掉的豆子
    public int score = 0;//得分

    private void Awake()
    {
        _instance = this;//单例变量
        Screen.SetResolution(1024, 768, false);//设置屏幕的大小
        int tempCount = rawIndex.Count;//鬼的数量
        for (int i = 0; i < tempCount; i++)
        {
            int tempIndex = Random.Range(0, rawIndex.Count);//在0，1，2，3中生成一个随机数
            usingIndex.Add(rawIndex[tempIndex]);//在已经使用的变量中添加该随机数
            rawIndex.RemoveAt(tempIndex);//从未使用的索引中去除该数
        }
        foreach (Transform t in GameObject.Find("Maze").transform)//寻找到maze对象
        {
            pacdotGos.Add(t.gameObject);//在迷宫中添加豆子
        }
        pacdotNum = GameObject.Find("Maze").transform.childCount;//获得豆子总数
    }
//start函数
    private void Start()
    {
         StopBtn.onClick.AddListener(delegate(){GetComponent<AudioSource>().Stop();});//暂停音乐播放
          PlayBtn.onClick.AddListener(delegate(){GetComponent<AudioSource>().Play();});//开启音乐播放
        SetGameState(false);//设置所有人物不能移动
    }
//状态更新函数
    private void Update()
    {
        if (nowEat == pacdotNum && pacman.GetComponent<PacmanMove>().enabled != false)//吃完所有的豆子而且吃豆人可以移动
        {
            gamePanel.SetActive(false);//游戏对象不激活
            Instantiate(winPrefab);//复制该对象
            StopAllCoroutines();//停止所有物体的协程
            SetGameState(false);//设置所有的人物为不能移动
        }
        if (nowEat == pacdotNum)//当前吃的豆子等于总数
        {
            if (Input.anyKeyDown)//如果按下任意键
            {
                SceneManager.LoadScene(0);//重载初始场景
            }
        }
        if (gamePanel.activeInHierarchy)//游戏对象在场景内是否显示
        {
            remainText.text = "Remain:\n\n" + (pacdotNum - nowEat);//输出剩余的豆子总数
            nowText.text = "Eaten:\n\n" + nowEat;//当前吃掉的豆子总数
            scoreText.text = "Score:\n\n" + score;//输出当前的得分
        }
    }
//start按键功能
    public void OnStartButton()
    {
        StartCoroutine(PlayStartCountDown());//显示倒计时
        AudioSource.PlayClipAtPoint(startClip, new Vector3(0, 0, -5));//播放开始音乐
        startPanel.SetActive(false);//start按钮不激活
        GetComponent<AudioSource>().Stop();//停止播放开始的音效
    }
//exit按键功能
    public void OnExitButton()
    {
        Application.Quit();//退出游戏
    }
//游戏开始倒计时
    IEnumerator PlayStartCountDown()
    {
        GameObject go = Instantiate(startCountDownPrefab);//复制倒计时对象
        yield return new WaitForSeconds(4f);//等待四秒钟
        Destroy(go);//销毁该对象
        SetGameState(true);//人物可以移动
        Invoke("CreateSuperPacdot", 10f);//10s后生成超级豆
        gamePanel.SetActive(true);//游戏对象激活
        GetComponent<AudioSource>().Play();//播放音乐
    }
//吃豆子
    public void OnEatPacdot(GameObject go)
    {
        nowEat++;//当前吃到的豆子数+1
        score += 100;//分数增加100分
        pacdotGos.Remove(go);//将豆子移除
    }
//吃到道具
    public void OnEatSuperPacdot()
    {
        score += 200;//当前分数加200分
        Invoke("CreateSuperPacdot", 10f);//调用生成超级豆函数，且时间在调用的10s以后
        isSuperPacman = true;//设置状态为超级吃豆人
        FreezeEnemy();//调用冻结敌人状态函数
        StartCoroutine(RecoveryEnemy());//在上述函数结束后调用回复函数
    }
//恢复敌人状态
    IEnumerator RecoveryEnemy()
    {
        yield return new WaitForSeconds(3f);//等待三秒
        DisFreezeEnemy();//调用解冻敌人函数
        isSuperPacman = false;//取消超级吃豆人的状态
    }
//创建超级吃豆人函数
    private void CreateSuperPacdot()
    {
        if (pacdotGos.Count < 5)//豆子数量小于5不生成道具
        {
            return;
        }
        int tempIndex = Random.Range(0, pacdotGos.Count);//获得一个随机的位置
        pacdotGos[tempIndex].transform.localScale = new Vector3(3, 3, 3);
        pacdotGos[tempIndex].GetComponent<Pacdot>().isSuperPacdot = true;//将对应位置的豆子变为超级豆
    }
//冻结敌人函数
    private void FreezeEnemy()
    {
        blinky.GetComponent<GhostMove>().enabled = false;
        clyde.GetComponent<GhostMove>().enabled = false;
        inky.GetComponent<GhostMove>().enabled = false;
        pinky.GetComponent<GhostMove>().enabled = false;//鬼无法移动
        blinky.GetComponent<SpriteRenderer>().color = new Color(0.7f, 0.7f, 0.7f, 0.7f);
        clyde.GetComponent<SpriteRenderer>().color = new Color(0.7f, 0.7f, 0.7f, 0.7f);
        inky.GetComponent<SpriteRenderer>().color = new Color(0.7f, 0.7f, 0.7f, 0.7f);
        pinky.GetComponent<SpriteRenderer>().color = new Color(0.7f, 0.7f, 0.7f, 0.7f);//设置为新的颜色透明度，使颜色发生变化
    }
//解冻敌人函数
    private void DisFreezeEnemy()
    {
        blinky.GetComponent<GhostMove>().enabled = true;
        clyde.GetComponent<GhostMove>().enabled = true;
        inky.GetComponent<GhostMove>().enabled = true;
        pinky.GetComponent<GhostMove>().enabled = true;//鬼可以移动
        blinky.GetComponent<SpriteRenderer>().color = new Color(1f, 1f, 1f, 1f);
        clyde.GetComponent<SpriteRenderer>().color = new Color(1f, 1f, 1f, 1f);
        inky.GetComponent<SpriteRenderer>().color = new Color(1f, 1f, 1f, 1f);
        pinky.GetComponent<SpriteRenderer>().color = new Color(1f, 1f, 1f, 1f);//鬼的颜色和透明度恢复为初始状态
    }
//人物游戏状态设置
    private void SetGameState(bool state)
    {
        pacman.GetComponent<PacmanMove>().enabled = state;//吃豆人移动状态设置
        blinky.GetComponent<GhostMove>().enabled = state;
        clyde.GetComponent<GhostMove>().enabled = state;
        inky.GetComponent<GhostMove>().enabled = state;
        pinky.GetComponent<GhostMove>().enabled = state;//鬼移动状态设置
    }
}
