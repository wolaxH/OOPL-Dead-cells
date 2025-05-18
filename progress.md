# 2025/05/16

## 今天做了
- 武器的分段傷害與有效frames
- Zombie 的基本Attacked()(未完成)

## 待辦
- 完成Zombie::Attacked()實作並測試其功能
- 製作盾牌盾反與減傷效果

## 想法
- 設置一個玩家的減傷值，若有舉盾則玩家減傷值不為零(根據盾牌數值調整)，並在Player::Attacked()中做傷和修正
- Player::Attack 可能需要做調整，或者建立一個UseWeapon functio 當使用的武器不同則進入不同function 如Attacke or block or skoot etc.

## NOTE
- 可能不做暈眩條系統