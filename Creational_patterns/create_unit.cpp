#include <iostream>
using namespace std;
/*
Общие цели архитектуры:
        Лёгкое добавление в игру новых фракций, родов войск и конкретных юнитов
        Минимизация дублирования кода
        Одинаковоё обращение к любому юниту во всём коде
Мотивации для создания абстрактной фабрики:
        Хотим создавать уникальные юниты, не перебирая при этом всех уникальных юнитов в игре
        Хотим иметь общий интерфейс создания различных юнитов для различных фракций
        Хотим быстро добавлять новые фракции в игру
Мотивации для создания такой иерархии юнитов:
        Хотим одинаково обращаться ко всем юнитам
        Хотим стандартифицировать основные поведения юнитов (перемещание, атака и т.п.)
        Но чтобы рода войск и конкретные их представители имели уникальные способности и значения характеристик
        Хотим относительно быстро добавлять новых уникальных юнитов и рода войск в игру
}*/


//подумать о добавлении остальных переменных как неконстантных
struct Unit {
    int Unit_HP; // хп конкретного юнита
    const int Max_HP; // максимальны хп данного вида юнитов
    const int Damage; // атака данного вида юнитов
    const int Combat_Distance; // максимальное расстояние атаки данного вида юнитов
    const int Speed; // скорость данного вида юнитов
    pair<int,int> Location; // местонахождение конкретного юнита
    Unit(int h, int d, int c_d, int s): Unit_HP(h), Max_HP(h), Damage(d), Combat_Distance(c_d), Speed(s), Location(make_pair(-1,-1)){};
    virtual void class_skill() = 0;
    virtual void special_skill() = 0;
    void set_start_square(pair<int,int> creation_city) { // задаёт квадрат, на котором юнит вступает в игру
        Location.first = creation_city.first;
        Location.second = creation_city.second;
    }
    void move(pair<int,int> to) {}
    void Attack(pair<int,int> to) {}
    virtual ~Unit() {}
};

// Базовые классы всех возможных видов юнитов
struct Infantryman: public Unit {
    Infantryman(int h, int d, int s):  Unit(h, d, 1, s){}
    virtual void special_skill() = 0;
    void class_skill() {
        cout << "InfantrymanSkill" << endl;
    }
    virtual ~Infantryman() {}
};

struct Archer: public Unit {
    Archer(int h, int d, int s):  Unit(h, d, 3, s){}
    virtual void special_skill() = 0;
    void class_skill() {
        cout << "ArcherSkill" << endl;
    }
    virtual ~Archer() {}
};

struct Knight: public Unit {
    Knight(int h, int d, int s):  Unit(h, d, 1, s){}
    virtual void special_skill() = 0;
    void class_skill() {
        cout << "KnightSkill" << endl;
    }
    virtual ~Knight() {}
};

// Конкретные классы стандартных юнитов
struct StandartInfantryman: public Infantryman {
    StandartInfantryman(): Infantryman(12, 4, 2){}
    void special_skill() {
        cout << "StandartInfantrymanSkill" << endl;
    }
};

struct StandartArcher: public Archer {
    StandartArcher():Archer(8, 2, 3){}
    void special_skill() {
        cout << "StandartArcherSkill" << endl;
    }
};

struct StandartKnight: public Knight {
    StandartKnight(): Knight(15, 6, 6){}
    void special_skill() {
        cout << "StandartKnightSkill" << endl;
    }
};

// Конкретные классы уникальных юнитов
struct EnglandArcher: public Archer {
    EnglandArcher(): Archer(8, 3, 3){}
    void special_skill() {
        cout << "EnglandArcherSkill" << endl;
    }
};


struct FranceKnight: public Knight {
    FranceKnight(): Knight(18, 6, 5){}
    void special_skill() {
        cout << "FranceKnightSkill" << endl;
    }
};

// Абстрактная фабрика для производства воинов
struct UnitFactory {
    virtual Unit* CreateInfantryman() = 0;
    virtual Unit* CreateArcher() = 0;
    virtual Unit* CreateKnight() = 0;
    virtual ~UnitFactory() {}
};

// Фабрика для создания воинов Английской армии
struct EnglandUnitFactory: public UnitFactory {
    Unit* CreateInfantryman(){
        return new StandartInfantryman;
    }
    Unit* CreateArcher(){
        return new EnglandArcher;
    }
    Unit* CreateKnight(){
        return new StandartKnight;
    }
};


// Фабрика для создания воинов Французской армии
struct FranceUnitFactory: public UnitFactory {
    Unit* CreateInfantryman(){
        return new StandartInfantryman;
    }
    Unit* CreateArcher(){
        return new StandartArcher;
    }
    Unit* CreateKnight(){
        return new FranceKnight;
    }
};

UnitFactory* CreateUnitFactory(int fraction_id) {
    switch(fraction_id) {
        case 0:
            return new EnglandUnitFactory;
        case 1:
            return new FranceUnitFactory;
    }
}

Unit* CreateUnit(int unit_id, UnitFactory* unit_creator) {
    switch(unit_id) {
        case 0:
            return unit_creator->CreateInfantryman();
        case 1:
            return unit_creator->CreateArcher();
        case 2:
            return unit_creator->CreateKnight();
    }
}
//fraction_id: Англия-0 Франция-1
//units_id: пех-0 лук-1 конь-2
// в идеале у меня где-то хранятся вектора с этими id

int main() {
    //пример работы
    int fraction_id;
    int unit_id;
    cin >> fraction_id >> unit_id;//на вход подаются фракция и тип юнита
    UnitFactory* unit_creator = CreateUnitFactory(fraction_id);//в начале игры создаём фабрику для этой фракции
    Unit* warrior = CreateUnit(unit_id, unit_creator);// которая делает нам конкретных юнитов
    warrior->class_skill();
    warrior->special_skill();
    cout << "Location:" << warrior->Location.first << " " << warrior->Location.second << " ";
    cout << "Damage and Speed:" << warrior->Damage << " " << warrior->Speed << " ";
    warrior->set_start_square(make_pair(5,5));
    cout << "New Location:" << warrior->Location.first << " " << warrior->Location.second << " ";
    return 0;
}