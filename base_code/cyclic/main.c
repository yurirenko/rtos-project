int main(int argc, char *argv[])
{
    struct periodic_task *pt;
    int t = 0;

    pt = start_periodic_timer(1000000, MINOR_CYCLE * 1000);
    if (pt < 0) {
        perror("Start Periodic Timer");

        return -1;
    }

    while(1) {
        uint8_t scheduled_tasks;

        wait_next_activation(pt);

        scheduled_tasks = scheduling_table[t / MINOR_CYCLE];