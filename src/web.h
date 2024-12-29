#ifndef WEB_H
#define WEB_H

void create_index_html(char *name, char *client, char *project,
                       char *description, char *duration);
void create_start_timer_html();
void create_edit_html(char *name, char *client, char *project,
                            char *description, char *start_time, char *end_time);
void create_stop_timer_html(char *name, char *client, char *project,
                            char *description, char *start_time, char *end_time,
                            char *final_duration);
void clean_up();

#endif // !WEB_H
