#pragma once

#include "Wall.h"
#include "Bullet.h"
#include "GameMap.h"




class CGamePathFinder
{
public:
    CGamePathFinder(const std::vector<std::vector<CGameMapElement>> &arr) : m_arr(arr)
    {
        m_col = m_arr[0].size();
        m_row = m_arr.size();
    }

    ~CGamePathFinder()
    {
    }

    /// ///////////////////////////////////////////////////////////////////////////////
    /// 路径 搜索

    // 两点之间的寻路:判断两个地图块之间是否连通
    // 使用广度优先算法，查找最小路径
    // x,y:起始位置
    // dx,dy:最终位置
    virtual void FindPath(int startX, int startY, int targetX, int targetY, VPath &path) = 0;

protected:

    int m_targetX, m_targetY;
    // 地图的列数
    // 地图的行数
    int m_col, m_row;


#define MAX_STEP 99999999
    //记录最小步数 : 初始设置一个很大的值，如果查找完值还是这个值，则说明不是连通的
    int m_minStep{ MAX_STEP };
    // 记录 哪些路径是否走过 : 走过=true,否则=false
    std::vector<std::vector<bool>> m_mark;
    // 记录 临时搜索路径
    VPath m_path;
    // 地图的索引
    const std::vector<std::vector<CGameMapElement>> &m_arr;
};

// 使用广度优先搜索
class CPathFinderBFS : public CGamePathFinder
{
public:
    CPathFinderBFS(const std::vector<std::vector<CGameMapElement>> &arr) : CGamePathFinder(arr)
    {

    }

    ~CPathFinderBFS()
    {
    }

    /// ///////////////////////////////////////////////////////////////////////////////
    /// 路径 搜索

    // 使用广度优先算法，查找最小路径
    // x,y:起始位置
    // dx,dy:最终位置
    virtual void FindPath(int startX, int startY, int targetX, int targetY, VPath &path)
    {
        m_targetX = targetX, m_targetY = targetY;
        bfs(startX, startY, targetX, targetY, path);
    }

private:
    //记录路径
    class CNode
    {
    public:
        CNode()
            : x(0)
            , y(0)
            , s(0)
            , parent(nullptr)
        {
        }

        CNode(int _x, int _y, int _s, CNode *p = nullptr)
            : x(_x)
            , y(_y)
            , s(_s)
            , parent(p)
        {
        }


        CNode(const CNode &rhs)
        {
            if(&rhs != this) {
                this->x = rhs.x;
                this->y = rhs.y;
                this->s = rhs.s;
                this->parent = rhs.parent;
            }
        }
        CNode &operator=(const CNode &rhs)
        {
            if(&rhs != this) {
                this->x = rhs.x;
                this->y = rhs.y;
                this->s = rhs.s;
                this->parent = rhs.parent;
            }
            return *this;
        }
        //~CNode() {
        //    if (parent) {
        //        delete parent;
        //        parent = nullptr;
        //    }
        //}

        int x, y; // 坐标
        int s; // 步数
        shared_ptr<CNode> parent; // 父结点
    };

    // 定义深度优先搜索函数
    //  x,y 当前所在的行，列
    bool bfs_item(int x, int y)
    {
        bool bFind = false;
        // 存讲未访问的可连通的结点
        std::queue<CNode> q_open;
        // 起点加入到队列中
        q_open.push(CNode(x, y, 0, nullptr));
        // 标记走过了
        m_mark[x][y] = true;
        // 当队列不为空的时候
        while(!q_open.empty()) {
            // 头部的出队，并检查结果
            auto curr = q_open.front();
            q_open.pop();
            // 到达目的地
            if(curr.x == m_targetX && curr.y == m_targetY) {
                m_minStep = curr.s;
                CNode *p = &curr; //当前点就是目的地
                while(p) {
                    PathNode n;
                    n.x = p->x;
                    n.y = p->y;
                    m_path.push_back(n);
                    p = p->parent.get();
                }
                bFind = true;
                while (!q_open.empty()) {
                    q_open.pop();
                }
                break;
            }
            // 遍历周边的点,放入队列
            {
                // 当前位置
                auto const &ele = m_arr[curr.x][curr.y];
                /// 分别判断四个方向是否可以走
                //左
                {
                    // 坐标为
                    int nx = curr.x;
                    int ny = curr.y - 1;
                    if(!(nx < 0 || nx >= m_row || ny < 0 || ny >= m_col) && m_mark[nx][ny] == false) {
                        if(!ele.m_bLeft && !m_arr[nx][ny].m_bRight) {
                            q_open.push(CNode(nx, ny, 1 + curr.s, new CNode(curr)));
                            m_mark[nx][ny] = true;
                        }
                    }
                }
                //上
                {
                    // 坐标为
                    int nx = curr.x - 1;
                    int ny = curr.y;
                    if(!(nx < 0 || nx >= m_row || ny < 0 || ny >= m_col) && m_mark[nx][ny] == false) {
                        if(!ele.m_bTop && !m_arr[nx][ny].m_bBottom) {
                            q_open.push(CNode(nx, ny, 1 + curr.s, new CNode(curr)));
                            m_mark[nx][ny] = true;
                        }
                    }
                }
                //右
                {
                    // 坐标为
                    int nx = curr.x;
                    int ny = curr.y + 1;
                    if(!(nx < 0 || nx >= m_row || ny < 0 || ny >= m_col) && m_mark[nx][ny] == false) {
                        if(!ele.m_bRight && !m_arr[nx][ny].m_bLeft) {
                            q_open.push(CNode(nx, ny, 1 + curr.s, new CNode(curr)));
                            m_mark[nx][ny] = true;
                        }
                    }
                }
                //下
                {
                    // 坐标为
                    int nx = curr.x + 1;
                    int ny = curr.y;
                    if(!ele.m_bBottom && !m_arr[nx][ny].m_bTop) {
                        if(!(nx < 0 || nx >= m_row || ny < 0 || ny >= m_col) && m_mark[nx][ny] == false) {
                            q_open.push(CNode(nx, ny, 1 + curr.s, new CNode(curr)));
                            m_mark[nx][ny] = true;
                        }
                    }
                }
            }
        } // end while(!q_open.empty())

        return bFind;
    };

    // dfs 深度优先搜索
    bool bfs(int x, int y, int m_targetX, int m_targetY, VPath &path)
    {
        // 初始化标记数组 : 开始全部未走过
        m_mark.clear();
        for(int i = 0; i < m_row; ++i) {
            vector<bool> vLine(m_col, false);
            m_mark.push_back(vLine);
        }
        // 路径清空
        m_path.clear();

        // 初始化标记数组 : 开始全部未走过
        m_nodes.clear();
        for(int x = 0; x < m_row; ++x) {
            vector<CNode> vLine;
            for(int y = 0; y < m_row; ++y) {
                CNode node;
                node.x = x;
                node.y = y;
                node.s = 0;
                vLine.push_back(node);
            }
            m_nodes.push_back(vLine);
        }




        // 从起点开始查找 : 标记起点坐标己经在路径中，防止后面重复走
        m_mark[x][y] = true;
        // 开始 深度优先搜索
        bool bFind = bfs_item(x, y);
        if(bFind) {
            std::copy(m_path.rbegin(), m_path.rend(), back_inserter<VPath>(path));
        }

        return bFind;
    };
    vector<vector<CNode>> m_nodes;
};



