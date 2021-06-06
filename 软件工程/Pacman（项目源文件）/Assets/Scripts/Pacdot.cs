using UnityEngine;

public class Pacdot : MonoBehaviour
{
    public bool isSuperPacdot = false;
//触发碰撞
    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.gameObject.name == "Pacman")//碰撞对象是吃豆人
        {
            if (isSuperPacdot)//是超级吃豆人
            {
                GameManager.Instance.OnEatPacdot(gameObject);//复制吃豆人对象
                GameManager.Instance.OnEatSuperPacdot();//复制超级吃豆人对象
                Destroy(gameObject);//销毁对象
            }
            else
            {
                GameManager.Instance.OnEatPacdot(gameObject);//复制吃豆对象
                Destroy(gameObject);//销毁吃豆对象
            }
        }
    }
}
