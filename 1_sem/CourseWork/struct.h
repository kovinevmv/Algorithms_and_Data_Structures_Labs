/* Структура "Музыкальная композиция */
struct MusicalComposition {
        char name[80];     // Название композиции 
        char author[80];   // Автор композиции 
        int year;          // Год написания композиции 
        struct MusicalComposition* next; // Указатель на следующий элемент 
        struct MusicalComposition* prev; // Указатель на предыдущий элемент 
};

typedef struct MusicalComposition MusicalComposition; //Объявление типа данных MusicalComposition

