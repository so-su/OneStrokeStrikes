#include "AlphaEnemy.hpp"

AlphaEnemy::AlphaEnemy():Polyomino(Size{8,20},Size{30,30},Point{700,150}){
    initialize();
}

void AlphaEnemy::initialize(){
    Polyomino::initialize(Size{20, 8}, 100, Cell::Black);
    new_shape_initialize();
}

void AlphaEnemy::new_shape_initialize(){
    perimeters.clear();
    
    // 幅優先探索して連結成分ごとに見ていき、それぞれの外周を求める
    num_component=0;
    Grid<bool> seen(grid_size.y,grid_size.x,false);
    Array<Point> tmp;
    Grid<std::array<Optional<Point>,4>> graph(grid_size.y+1,grid_size.x+1);
    
    for(auto [i,j]:step(grid_size)){
        if(not is_filled(i,j) or seen[i][j]){
            continue;
        }
        
        // ここを抜けるごとに新しい連結成分を処理
        
        for(auto &ary:graph){
            ary.fill(none);
        }
        
        Point component_upper_left{grid_size};
        
        seen[i][j]=true;
        tmp.emplace_back(i,j);
        
        while(not tmp.empty()){
            auto [x,y]=tmp.back();
            tmp.pop_back();
            
            if(y<component_upper_left.y){
                component_upper_left={x,y};
            }
            else if(y==component_upper_left.y){
                component_upper_left.x=Min(x,component_upper_left.x);
            }
            
            if(not is_filled(x,y-1)){
                graph[x][y][0]=Point{x+1,y};
            }else if(not seen[x][y-1]){
                seen[x][y-1]=true;
                tmp.emplace_back(x,y-1);
            }
            if(not is_filled(x+1,y)){
                graph[x+1][y][1]=Point{x+1,y+1};
            }else if(not seen[x+1][y]){
                seen[x+1][y]=true;
                tmp.emplace_back(x+1,y);
            }
            if(not is_filled(x,y+1)){
                graph[x+1][y+1][2]=Point{x,y+1};
            }else if(not seen[x][y+1]){
                seen[x][y+1]=true;
                tmp.emplace_back(x,y+1);
            }
            if(not is_filled(x-1,y)){
                graph[x][y+1][3]=Point{x,y};
            }else if(not seen[x-1][y]){
                seen[x-1][y]=true;
                tmp.emplace_back(x-1,y);
            }
        }
        // 一行目の最左の点をスタート点としてパスをつくる
        Point now=component_upper_left;
        int32 prev=3;
        perimeters.resize(num_component+1); // TODO よい書き方？
        do{
            for(int32 d=1;d<=4;++d){
                int32 next=(prev+d)%4;
                if(graph[now.x][now.y][next].has_value()){
                    perimeters[num_component].emplace_back(Line{upper_left+now*cell_size.x,upper_left+*graph[now.x][now.y][next]*cell_size.x});
                    now=*graph[now.x][now.y][next];
                    prev=next;
                    break;
                }
            }
        }while(now!=component_upper_left);
        
        ++num_component;
    }
    
    gauge_lens.assign(num_component,0.0);
}

void AlphaEnemy::get_damaged(size_t remove_num){
    // 埋まっているセルの中から remove_num 個選んで削除する
    Array<Point> filled_cells;
    for(auto [i,j]:step(grid_size)){
        if(cells[i][j]!=Cell::None){
            filled_cells.emplace_back(i,j);
        }
    }
    auto cells_removed=filled_cells.choice(Min(remove_num,size(filled_cells)));
    for(auto [i,j]:cells_removed){
        cells[i][j]=Cell::None;
        rects[i][j]=none;
        --cell_num;
    }
    
    new_shape_initialize();
}

bool AlphaEnemy::alive()const{
    return cell_num>0;
}

int32 AlphaEnemy::attack_value()const{
    return 10;
}

// ゲージを更新し、満タンになったらtrueを返す
int32 AlphaEnemy::update_gauges(bool speed_up){
    int32 full=0;
    for(auto gauge_idx:step(num_component)){
        gauge_lens[gauge_idx]+=Scene::DeltaTime()*(speed_up?10.0:1.0);
    
        if(static_cast<size_t>(gauge_lens[gauge_idx])==std::size(perimeters[gauge_idx])){
            gauge_lens[gauge_idx]=0.0;
            ++full;
        }
    }
    return full;
}

// 外周のゲージを描画する
void AlphaEnemy::draw_gauges()const{
    for(auto gauge_idx:step(num_component)){
        int32 len_int=static_cast<int32>(gauge_lens[gauge_idx]);
        for(auto i:step(len_int)){
            perimeters[gauge_idx][i].draw(5,Palette::Orange);
        }
        perimeters[gauge_idx][len_int].stretched(0,cell_size.x*((gauge_lens[gauge_idx]-len_int)-1.0)).draw(5,Palette::Orange);
    }
}
