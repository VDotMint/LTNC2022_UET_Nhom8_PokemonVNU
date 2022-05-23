# Pokémon UET
Formerly Pokémon VNU but that is only possible if LTNC is the only subject in the semester.

## Giới thiệu về Game:
- Pokémon UET là một game mang tính tái tạo lại một phần nhỏ của tựa game Pokémon Emerald của GAME FREAK trong ngôn ngữ lập trình C++ và SDL2.0.
- Những map của game được thiết kế lại để chúng nhìn giống 2 tòa nhà chính của Trường Đại học Công Nghệ, ĐHQGHN (nhà G2 và nhà E3).
- Nhà G2 đã được trưng dụng làm một cơ sở để người chơi chiến đấu Pokémon mới một loạt NPC trong Game.
- Nhà E3, dù vẫn mang tính chất của khu nhà E3 vốn có, giờ có mục tiêu chính là giúp người chơi thuê mượn Pokémon.
- Mục tiêu chính của game là sử dụng 3 con Pokémon của người chơi và battle qua nhiều phòng trong nhà G2 nhất có thể, sau mỗi battle, điểm số sẽ được tích lũy dần lên.
- Ngoài battle ra thì trong game cũng có những cái NPC và những object với những trao đổi và than thở... gì gì đó.


## Các câu lệnh để chạy game
**QUAN TRỌNG: Đảm bảo là phiên bản g++ phải là từ v8.1.0 trở lên (kiểm tra bằng cách gõ lệnh g++ --version)**
### Cách 1: Chạy file run.bat
### Cách 2: 
- Git clone hoặc download file zip về. Ở trong thư mục /LTNC2022_UET_Nhom8_PokemonVNU, mở CMD
- Copy và chạy câu lệnh sau: 
> g++ -fdiagnostics-color=always -Wall -Wl,-subsystem,windows CPPs\\*.cpp -o build/game -Iinclude/SDL2 -Iinclude -Llib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

> cd build

> game


## Phân công nhiệm vụ trong nhóm
- Ngô Danh Lam: Phát triển cấu trúc Pokémon và hệ thống battle; Xây dựng những hàm và cấu trúc cơ sở của game.
- Lê Vũ Minh: Phát triển engine map, camera, NPC, audio, các Menu và nút bấm trong game, xây dựng frontend của battle.
- Lê Huy Tuấn Anh: Xử lý đồ họa, nhập liệu cho game, phát triển Text, Dialogue.

## Instruction
![image](https://user-images.githubusercontent.com/29592868/169816056-b2d8eeec-f55c-4a95-84ab-6aee60790fa6.png)

## Typing
![image](https://user-images.githubusercontent.com/29592868/169819959-e4ae407b-d83b-4869-bb7a-b08ab931670c.png)

