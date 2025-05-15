# 2025/05/16

## 今天做了
- TwinDagger 的揮舞動畫及攻擊段數的傷害
- Bone 的揮舞動畫

## 待辦
- 研究一下TwinDaggers, Bone 的動畫跟傷害，調整這兩個的碰撞箱
- 盾牌系統，包含頓反跟一般格擋

## 想法
- 有個接口如果怪物在攻擊state and Anim fram 為特定的fram then mob->Attacked
- 設置一個玩家的減傷值，若有舉盾則玩家減傷值不為零(根據盾牌數值調整)，並在Player::Attacked()中做傷和修正
- Player::Attack 可能需要做調整，或者建立一個UseWeapon functio 當使用的武器不同則進入不同function 如Attacke or block or skoot etc.


## NOTE
- 可能不做暈眩條系統