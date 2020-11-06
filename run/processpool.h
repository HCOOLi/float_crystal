template<typename T>
class shmqueue{
private:
    sem_t * _sem;
    T  *_vector;
    int shmid;
    string name;
    struct data
    {
       int _capacity;
        int _size;
        int head;
        int end;
    };
    

public:
    shmqueue(string name ){

        _vector=new(shm) T[200];
        

    }
    





}


class ProcessPool{
    private:



    public:
}