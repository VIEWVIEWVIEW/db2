struct Index
{
    char Ordnungsbegriff[21];
    int Position;
};

struct Buch
{
    char Autor[21] = {0};
    char Titel[21] = {0};
    char Verlagsname[21] = {0};
    int Erscheinungsjahr;
    char Erscheinungsort[41] = {0};
    char ISBN[15] = {0};
};