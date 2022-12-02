3조 동작 매뉴얼

	플레이씬
   	조작방법
   ∙ 이동키 : w a s d
   ∙ 대쉬키 : 스페이스바
   ∙ 공격키 : 마우스 좌 클릭, 우 클릭
   ∙ 스킬키 : q, e, r
   ∙ 보스 체력 감소 : q
   ∙ 퍼즈 및 설정창 : esc
   	주의사항
   ∙ 플레이씬에서 타이틀로 이동 시 플레이씬 초기화가 안 됨
   ∙ 스킬 버튼 아직 연동 안 됨
   ∙ 몬스터와 플레이어 전투 미 구현(보스의 발차기 패턴만 전투 적용 됨)

	맵 툴 씬
   	타일구분
   ∙ Tile : 바닥
   ∙ Wall : 벽, 플레이어와 에너미 등 이동 제한을 위한 구역 설정
   ∙ Object : 장식물 및 철창
   ∙ Player : 플레이어 
   ∙ Enemy : 적 4종
   ∙ Sector : 방 개념(sector를 지정해서 sector 내부의 모든 타일들을 묶어주는 역할)
   ∙ Cliff : 추락 구간 범위 지정
   	조작방법
   ∙ 타일설치 및 선택 : 마우스 좌 클릭
   ∙ 타일 선택 해제 : 마우스 우 클릭
   ∙ 격자 이동 가능 : 마우스 휠 클릭
   ∙ 줌인/줌아웃 : 마우스 휠
   ∙ SACTER 전체 지우기 : F6
   ∙ CLIFF 전체 지우기 : F7
   	상세사항
   ∙ 우측 타일 팔레트 상단 흰색 글씨를 누르면 tile, wall, object 등 타일 종류가 변경됨
   ∙ 우측 타일 팔레트에서 원하는 타일 클릭 시 마우스 상단에 설치 대상이 나타나며, 그 상태에서 격자에 클릭 혹은 드래그 시 설치 가능(드래그는 시작 지점부터 끝지점까지의 사각형 형태로 타일 채우기 가능)
   ∙ 마우스 우클릭 시 마우스 상단에 설치 대상이 사라지며 그 상태로 설치되어 있는 타일을 클릭하면 삭제됨
   ∙ 좌측 상단에 SAVE 클릭 시 저장 창이 뜨며 키보드 입력으로 파일 이름 작성 후 창 하단 SAVE 버튼 누르면 저장됨
   ∙ 좌측 상단에 LOAD 클릭 시 저장 되어 있는 파일 목록이 뜨며 해당 이름을 클릭 하면 저장된 맵이 로드 됨, 다시 LOAD를 클릭하면 로드 창 꺼짐
   	주의사항
   ∙ 저장된 맵 LOAD 시 저장된 플레이어가 불러와지지 않는 오류가 있어 LOAD 후 다시 PLAYER 설치 필요
   ∙ 모든 타일들은 PlayScene에서 Sector로 구분되기 때문에 비는 타일 없이 전체 지정되어야 함
   ∙ 맵 SAVE 후 게임을 재부팅 시켜줘야 맵 적용 가능


	스킬 툴 씬

   	조작방법
   ∙ 이동키 : w, a, s, d
   ∙ 스킬사용 : 마우스 좌 클릭 
   	상세사항
   ∙ Skill: 단일 스킬
   ∙ SkillSet: 연계 스킬(플레이어만 사용)
   ∙ SkillToolScene에서 단일 스킬만 제작 가능 / SkillSet은 tables/SkillSet.csv 에서 연계할 스킬 작성
   	스킬제작 선택항목 구분
   1. SkillName : 스킬 이름
   2. Element : 원소 (Fire: 0, Air: 1, Earth: 2, Lightning: 3, Water: 4, Chaos: 5)
   3. AttackType : 공격 타입(Single(단타): 0, Multiple(연발): 1, SaveAttacks(충전형): 2)
      (연발 스킬은 쿨타임이 전부 지나야 사용 가능하지만, 충전형 스킬은 1개 이상만 충전되어도 스킬 사용 가능)
   4. AttackCntLim : 연발 스킬 최대 횟수
   5. AttackInterval : 연발 스킬 간격
   6. Distance : 스킬 사용 한계 거리 
      (한계 거리 내의 마우스 위치로 스킬 사용, 플레이어 점프 동작 시 distance 절반 이하의 거리에 마우스 포인터 위치하면 제자리 점프, 절반 이상의 거리에선 distance만큼 점프)
   7. AttackShape
      1) Surrounded: 플레이어와 겹쳐진 스킬(화염 버프 등)
      2) Range : 범위 스킬, FallingHeight 설정 시 범위 낙하 스킬(메테오 등) 사용 가능
         - Range 스킬은 해당 위치의 마법진에서 데미지 적용, 단일 낙하 스킬의 경우 마법진 가시화, 연발 스킬로 설정 시 마법진 비가시화 (인게임 설정 반영)
      3) Rotate : 플레이어 주위에 원운동 하는 스킬 사용 가능
      4) Wave : 파동 형태 또는 직선 형태의 스킬 사용 가능
   8. Amplitude
      1) Range 스킬의 경우 : 아래에 생기는 마법진의 반지름
      2) WaveType의 경우 : 파동 형태 공격의 진폭, 0 입력시 직선 형태로 이동
   9. Frequency
      1) Range 스킬의 경우 : 연발 스킬로 사용 시 반지름이 amplitude * frequency인 범위에 무작위 범위 공격
      2) WaveType의 경우 : 파동 형태 공격의 진동수로 값이 클 수록 자주 진동
   10. WaveType - WaveType일 때만 적용, 한 방향 공격 / 돌아오는 공격
   11. FallingHeight - RangeType일 때만 적용, 스킬 낙하 높이 지정
   12. RangeType
      1) Default : FallingHeight 0일 시 현재 플레이어의 위치에 스킬 사용(player 추적 x)
      2) VerticalDescent : 수직 강하 스킬(ex. 고드름 낙하 스킬)
      3) FromAbovePlayer : 플레이어 머리 위를 기준으로 해당 지점을 향한 기울어진 강하 스킬(ex. 메테오)
   13. StopMoving
      1) Movable : 스킬 사용 중 이동 가능
      2) Immovable : 스킬 사용 중 이동 불가
   14. SkillDelay : 스킬 선딜레이
   15. SkillCoolDown : 스킬 쿨타임
   16. DmgType
      1) Once : 단타
      2) Periodic : 다수 타격
   17. DmgRatio - 사용 개체 공격력 대비 데미지
   18. DmgDelay : 타격 딜레이
   19. Duration : 스킬 지속 시간
   20. Speed : 스킬 이동 속도
   21. AnimClipName 1, 2 : 애니메이션 클립 이름 (Csv로 제작 후 해당 클립 이름 작성)(1 : Wave의 경우 좌측 방향, 2 : Wave의 경우 우측 방향)
   22. SoundName 1, 2 : 스킬 사운드 (1 : 스킬 사용 시 사운드, 2 : 스킬 종료 사운드)

   	주의사항
   ∙ tables/skiltable.csv 연 상태로 Save Skill 버튼을 누르면 터짐(해당 프로그램에서 csv파일을 열어서 내용 수정 하기 때문에 해당 csv 파일이 열려 있으면 오류 발생)
