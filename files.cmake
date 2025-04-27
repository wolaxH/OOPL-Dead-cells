set(SRC_FILES
    App/AppMainPageInit.cpp
    App/AppMainPage.cpp
    App/AppInGame.cpp
    App/AppInGameInit.cpp

    Character/Character.cpp
    Character/Player.cpp
    Character/Zombie.cpp

    Menu/Menu.cpp
    SolidObj.cpp
)

set(INCLUDE_FILES
    App.hpp

    Character/Character.hpp
    Character/Player.hpp
    Character/Zombie.hpp
    Character/Mob.hpp

    Menu/Button.hpp
    Menu/Menu.hpp

    Abstract/MapObj.hpp

    MyUtil/Camera.hpp
    MyUtil/Timer.hpp

    Item/PickUp.hpp
    Item/Weapon/Weapon.hpp
    Item/Weapon/Knife.hpp

    SolidObj.hpp
    OneSidedPlatform.hpp  
    BackGround.hpp
    PlayerUI.hpp
    Hp.hpp

)

set(TEST_FILES
)
