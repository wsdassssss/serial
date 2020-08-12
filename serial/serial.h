#ifndef SERIAL_H
#define SERIAL_H

#include <QMainWindow>

//fyl
#include <QSerialPort>
#include <QDebug>

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QSlider>

namespace Ui {
class serial;
}

class serial : public QMainWindow
{
    Q_OBJECT

public:
    explicit serial(QWidget *parent = nullptr);
    ~serial();

private:
    Ui::serial *ui;
    void system_init();

public:
    //fyl
    int data_jiexi;
    QByteArray send_data;
    QByteArray recv_data;
    QSerialPort global_port;
    int data_line;
    int loc_ff;
private slots:

    void print();
    //接收数据处理
    void data_recv_deal();
    //数据传输
    void data_send();
    void data_send_init();
    void data_recv();
    void data_stop();
    //串口配置
    void on_btn_findport_clicked();
    void on_btn_portstatus_clicked();
    //基本控制
    void on_btn_leftandup_pressed();
    void on_btn_leftandup_released();

    void on_btn_up_pressed();
    void on_btn_up_released();

    void on_btn_rightandup_pressed();
    void on_btn_rightandup_released();

    void on_btn_left_pressed();
    void on_btn_left_released();

    void on_btn_right_pressed();
    void on_btn_right_released();

    void on_btn_leftanddown_pressed();
    void on_btn_leftanddown_released();

    void on_btn_down_pressed();
    void on_btn_down_released();

    void on_btn_rightanddown_pressed();
    void on_btn_rightanddown_released();

    void on_btn_bianbei_T_pressed();
    void on_btn_bianbei_T_released();
    void on_btn_bianbei_W_pressed();
    void on_btn_bianbei_W_released();

    void on_btn_jujiao_Far_pressed();
    void on_btn_jujiao_Far_released();
    void on_btn_jujiao_Close_pressed();
    void on_btn_jujiao_Close_released();

    void on_btn_light_big_pressed();
    void on_btn_light_big_released();
    void on_btn_light_small_pressed();
    void on_btn_light_small_released();


    void on_btn_rain_status_clicked();
    void on_btn_laser_clicked();
    //声波控制
    void on_btn_aco_clicked();
    void on_btn_aco_status_clicked();
    void on_btn_strongsound_status_clicked();
    void on_btn_alarm_clicked();
    void on_btn_alarm_status_clicked();
    void on_btn_gongfang_clicked();
    void on_btn_gongfang_status_clicked();
    void on_btn_micro_status_clicked();

    //播放控制
    void on_btn_music_go_clicked();
    void on_btn_music_last_clicked();
    void on_btn_music_status_clicked();
    void on_btn_music_next_clicked();
    void on_btn_music_back_clicked();
    void on_btn_music_circle_clicked();
    void on_btn_music_change_clicked();
    void on_btn_music_status_check_clicked();
    void on_btn_music_volume_clicked();
    void on_btn_music_voulume_config_clicked();
    void on_btn_recordsound_status_clicked();
    void on_btn_music_count_clicked();
    void on_btn_music_nowno_clicked();
    void on_btn_music_startnow_clicked();

    //设备异常查询
    void on_btn_device_check_clicked();

    //光线控制
    void on_btn_white_oepn_clicked();
    void on_btn_white_close_clicked();
    void on_btn_white_spark_clicked();
    void on_btn_white_search_clicked();
    void on_btn_white_config_clicked();
    void on_btn_red_open_clicked();
    void on_btn_red_close_clicked();
    void on_btn_red_spark_clicked();
    void on_btn_red_search_clicked();
    void on_btn_red_config_clicked();
    void on_btn_green_open_clicked();
    void on_btn_green_close_clicked();
    void on_btn_green_spark_clicked();
    void on_btn_green_search_clicked();
    void on_btn_green_config_clicked();
    void on_btn_blue_open_clicked();
    void on_btn_blue_close_clicked();
    void on_btn_blue_spark_clicked();
    void on_btn_blue_search_clicked();
    void on_btn_blue_config_clicked();
    void on_btn_laser_big_pressed();
    void on_btn_laser_big_released();
    void on_btn_laser_small_pressed();
    void on_btn_laser_small_released();
    void on_btn_laser_config_clicked();
    void on_btn_senddata_clicked();
    void on_btn_clear_clicked();
    void on_btn_mid_clicked();
    void on_btn_clear_recv_clicked();
};

#endif // SERIAL_H
