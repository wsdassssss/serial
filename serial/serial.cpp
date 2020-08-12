#include "serial.h"
#include "ui_serial.h"

//fyl
#include <QMessageBox>
serial::serial(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::serial)
{
    ui->setupUi(this);
    system_init();
    data_send_init();

    on_btn_findport_clicked();
}

serial::~serial()
{
    delete ui;
}
void serial::print(){
    qDebug()<<recv_data.length();
    for(int i=0;i<7;i++)
        qDebug()<<QVariant((unsigned char)recv_data[i]).toInt();
}
//初始化
void serial::system_init()
{
    connect(&global_port, &QSerialPort::readyRead, this, &serial::data_recv);
    //Button失能
    ui->btn_strongsound->setEnabled(false);
    ui->btn_micro->setEnabled(false);
    ui->btn_recordsound->setEnabled(false);
    ui->btn_senddata->setEnabled(false);

    //Menu失能
    /*
    ui->tab_2->setEnabled(false);
    ui->tab_3->setEnabled(false);
    ui->tab_4->setEnabled(false);
    */
    //TXT设置
    ui->lineEdit_aco->setEnabled(false);
    ui->lineEdit_aco->setEnabled(false);
    ui->lineEdit_strongsound->setEnabled(false);
    ui->lineEdit_alarm->setEnabled(false);
    ui->lineEdit_gongfang->setEnabled(false);
    ui->lineEdit_micro->setEnabled(false);
    ui->lineEdit_recordsound->setEnabled(false);
    ui->lineEdit_music_circle->setEnabled(false);
    ui->lineEdit_music_status->setEnabled(false);
    ui->lineEdit_music_count->setEnabled(false);
    ui->lineEdit_device_check->setEnabled(false);
    ui->lineEdit_aco->setEnabled(false);
    ui->btn_music_change->setEnabled(false);

    ui->btn_portstatus->setText("打开串口");

}
void serial::data_recv_deal()
{
    int deviceid=QVariant((unsigned char)recv_data[1]).toInt();
    int module=QVariant((unsigned char)recv_data[4]).toInt();
    int state=QVariant((unsigned char)recv_data[5]).toInt();
    int light=QVariant((unsigned char)recv_data[2]).toInt();//灯光ID
    QString temp=QString::number(state);

    //  1<=id<=4?
//    if((deviceid<1)||(deviceid>4)){
//        qDebug()<<"设备ID异常";
//            return;
//    }
    if(deviceid==2){
        //声波模块处理
        switch (module) {
        case 0:
            ui->textEdit_show->insertPlainText("接收--声波设备状态指令::");
            if(!((state==1)||(state==0))){
                ui->textEdit_show->insertPlainText("指令错误\n");
                return;
            }
            if(state==0){
                ui->lineEdit_aco->setText("关闭");
                ui->btn_aco_status->setText("打开");
                ui->textEdit_show->insertPlainText("成功--已关\n");
            }
            if(state==1){
                ui->lineEdit_aco->setText("打开");
                ui->btn_aco_status->setText("关闭");
                ui->textEdit_show->insertPlainText("成功--已开\n");
            }
            break;
        case 1:
         ui->textEdit_show->insertPlainText("接收--01警报音状态指令::");
         if(!((state==1)||(state==2))){
             ui->textEdit_show->insertPlainText("指令错误\n");
             return;
         }
         if(state==2){
             ui->lineEdit_alarm->setText("关闭");
             ui->btn_alarm_status->setText("打开");
             ui->textEdit_show->insertPlainText("已关 \n");
         }
         if(state==1){
             ui->lineEdit_alarm->setText("开启");
             ui->btn_alarm_status->setText("关闭");
             ui->textEdit_show->insertPlainText("已开 \n");
         }
            break;
        case 2:
         ui->textEdit_show->insertPlainText("接收--02播放模块指令\n");
         if(state<1||state>6)
         {
             ui->textEdit_show->insertPlainText("指令错误\n");
             return;
         }
         if(state==1){
            ui->textEdit_show->insertPlainText("上一曲\n");
            return;
         }
         if(state==2){
            ui->textEdit_show->insertPlainText("下一曲\n");
            return;
         }
         if(state==3){
            ui->textEdit_show->insertPlainText("停止\n");
            return;
         }
         if(state==4){
            ui->textEdit_show->insertPlainText("播放\n");
            return;
         }
         if(state==5){
            ui->textEdit_show->insertPlainText("单曲循环\n");
            return;
         }
         if(state==6){
            ui->textEdit_show->insertPlainText("列表循环\n");
            return;
         }
            break;
        case 3:
            ui->textEdit_show->insertPlainText("接收--03播放指定编号歌曲\n");
            //note::暂时不写
            //播放指定变化歌
            break;
        case 4:
            ui->textEdit_show->insertPlainText("接收--04音量控制模块\n");
            //note::暂时不写
            //音量控制
            break;
        case 5:
            ui->textEdit_show->insertPlainText("接收--05功放+强声模块::");
            if(state<1||state>4)
            {
                ui->textEdit_show->insertPlainText("指令错误\n");
                return;
            }
            if(state==1){
                ui->lineEdit_strongsound->setText("开启");
                ui->btn_strongsound_status->setText("关闭");
                ui->textEdit_show->insertPlainText("强声设备已开 \n");
                return;
            }
            if(state==2){
                ui->lineEdit_strongsound->setText("关闭");
                ui->btn_strongsound_status->setText("打开");
                ui->textEdit_show->insertPlainText("强声设备已关 \n");
                return;
            }
            if(state==3){
                ui->lineEdit_gongfang->setText("关闭");
                ui->btn_gongfang_status->setText("打开");
                ui->textEdit_show->insertPlainText("功放已关 \n");
                return;
            }
            if(state==4){
                ui->lineEdit_gongfang->setText("开启");
                ui->btn_gongfang_status->setText("关闭");
                ui->textEdit_show->insertPlainText("功放已开 \n");
                return;
            }
            break;
        case 6:
            //note::???干啥的
            //实时喊话 1-已开 0->已关
            ui->textEdit_show->insertPlainText("接收--06实时喊话模块::麦克风");
            if(state<0||state>1){
                ui->textEdit_show->insertPlainText("指令错误\n");
                return;
            }
            if(state==0){
                ui->lineEdit_micro->setText("关闭");
                ui->btn_micro_status->setText("开启");
                ui->textEdit_show->insertPlainText("已关\n");
                return;
            }
            if(state==1){
                ui->lineEdit_micro->setText("开启");
                ui->btn_micro_status->setText("关闭");
                ui->textEdit_show->insertPlainText("已开\n");
                return;
            }
            break;
        case 7:
            ui->textEdit_show->insertPlainText("接收--07喊话录音模块::");
            if(state<0||state>1){
                ui->textEdit_show->insertPlainText("指令错误\n");
                return;
            }
            if(state==0){
                ui->lineEdit_recordsound->setText("关闭");
                ui->btn_recordsound_status->setText("开启");
                ui->textEdit_show->insertPlainText("喊话录音已关 \n");
                return;
            }
            if(state==1){
                ui->lineEdit_recordsound->setText("开启");
                ui->btn_recordsound_status->setText("关闭");
                ui->textEdit_show->insertPlainText("喊话录音已开 \n");
                return;
            }
            break;
        case 17: //0x11
            //note::上面有个，这里暂时不写
            ui->textEdit_show->insertPlainText("接收--11警报音状态:: \n");
            break;
        case 18:
            ui->textEdit_show->insertPlainText("接收--12播放模块状态::");
            if(state<1||state>4){
               ui->textEdit_show->insertPlainText("指令错误 \n");
               return;
            }
            if(state==1){
                ui->btn_music_status->setText("播放");
                ui->lineEdit_music_status->setText("停止");
                ui->textEdit_show->insertPlainText("已停止 \n");
                return;
            }
            if(state==2){
                ui->btn_music_status->setText("停止");
                ui->lineEdit_music_status->setText("播放");
                ui->textEdit_show->insertPlainText("正在播放 \n");
                return;
            }
            if(state==3){
                ui->lineEdit_music_circle->setText("单曲循环");
                ui->textEdit_show->insertPlainText("单曲循环 \n");
                return;
            }
            if(state==4){
                ui->lineEdit_music_circle->setText("列表循环");
                ui->textEdit_show->insertPlainText("列表循环 \n");
                return;
            }
            break;
        case 19:
            ui->textEdit_show->insertPlainText("接收--13当前曲目编号::");
            ui->textEdit_show->insertPlainText(temp+"\n");
            ui->lineEdit_nowno->setText(temp);
            break;
        case 20:
            ui->textEdit_show->insertPlainText("接收--14当前音量:: \n");
            ui->textEdit_show->insertPlainText(temp+"\n");
            ui->lineEdit_music_volume->setText(temp);

            break;
        case 21:
            //note::不清楚先不整
            ui->textEdit_show->insertPlainText("接收--15::未定义 \n");

            break;
        case 22:
            ui->textEdit_show->insertPlainText("接收--16文件夹曲目总数:: \n");
            //数据位变为QString显示
            ui->textEdit_show->insertPlainText(temp+"\n");
            ui->lineEdit_music_count->setText(temp);
            break;
        case 24:
            ui->textEdit_show->insertPlainText("接收--18设备异常查询结果::");
            if(state<1||state>5){
                ui->textEdit_show->insertPlainText("指令错误！！ \n");
                return;
            }
            if(state==1){
                ui->textEdit_show->insertPlainText("功放模块异常\n");
                ui->lineEdit_device_check->setText("功放模块异常\n");
                return;
            }
            if(state==2){
                ui->textEdit_show->insertPlainText("声压级异常\n");
                ui->lineEdit_device_check->setText("声压级异常\n");
                return;
            }
            if(state==3){
                ui->textEdit_show->insertPlainText("电源模块异常\n");
                ui->lineEdit_device_check->setText("电源模块异常\n");
                return;
            }
            if(state==4){
                ui->textEdit_show->insertPlainText("温度异常\n");
                ui->lineEdit_device_check->setText("温度异常\n");
                return;
            }
            if(state==5){
            ui->textEdit_show->insertPlainText("存储器异常\n");
            ui->lineEdit_device_check->setText("存储器异常\n");
            return;
        }
            break;
        default:
            ui->textEdit_show->insertPlainText("指令模块不存在！！！");
            break;
        }
     }
    //只有[5]
    if(deviceid==3){
        if(light<0||light>3){
            ui->textEdit_show->insertPlainText("光线指令错误");
            return;
        }
        ui->textEdit_show->insertPlainText("光线控制指令：");
        switch (light)
        {
        case 0: //激光到指定位置
            //白光
            ui->textEdit_show->insertPlainText(temp+"\n");
            ui->textEdit_show->insertPlainText("白光功率获取成功\n");
            break;
        case 1:
            ui->textEdit_show->insertPlainText(temp+"\n");
            ui->textEdit_show->insertPlainText("红光功率获取成功\n");
            break;
        case 2:
            ui->textEdit_show->insertPlainText(temp+"\n");
            ui->textEdit_show->insertPlainText("绿光功率获取成功\n");
            break;
        case 3:
            ui->textEdit_show->insertPlainText(temp+"\n");
            ui->textEdit_show->insertPlainText("蓝光功率获取成功\n");
            break;
        default:
            ui->textEdit_show->insertPlainText("指令错误！！！\n");
            break;
        }
    }

}
//数据传输
void serial::data_send()
{
    qDebug()<<send_data.toHex();
    global_port.write(send_data);
    ui->textEdit_show->insertPlainText("send---"+send_data.toHex().toUpper()+"\n");
    qDebug()<<"Finish---data_send";
    data_send_init();
}
void serial::data_recv()
{
    recv_data.append(global_port.readAll());
    ui->textEdit_show->insertPlainText(recv_data.toHex()+"\n");
    qDebug()<<recv_data.length();
    qDebug()<<"now: "+recv_data.toHex();
    //print();
    //len>7?
    if(recv_data.length()>=7){
        int sum=0;
      //  QVariant((unsigned char)recv_data[1]).toInt()
        int rec_head=QVariant((unsigned char)recv_data[0]).toInt();
        int rec_sum=QVariant((unsigned char)recv_data[6]).toInt();
        qDebug()<<"sum checing checking";
        if(rec_head==255){
             for(int i=1;i<6;i++)
                 sum=sum+QVariant((unsigned char)recv_data[i]).toInt();
             qDebug()<<sum;
             qDebug()<<rec_sum;
             if(sum==rec_sum){
                 data_recv_deal();
                 qDebug()<<"dealing";
                 recv_data.clear();
             }else
             {
                 qDebug()<<"sum is wrong";
                 ui->textEdit_show->insertPlainText("sum is wrong \n");
                 recv_data.clear();
             }
        }
        else
        {
            ui->textEdit_show->insertPlainText("head is wrong \n");
            recv_data.clear();
        }
    }
   // ui->textEdit_show->insertPlainText(recv_data.toHex()+"\n");
    //qDebug()<<"Finsh---data_recv";
    //recv_data.clear();

}
void serial::data_stop()
{
    data_jiexi=ui->lineEdit_address->text().toInt();
    send_data[1]=char(data_jiexi);
    send_data[2]=0x00;
    send_data[3]=0x00;
    send_data[4]=0x00;
    send_data[5]=0x00;
    for(int i=1;i<6;i++)
        send_data[6]=send_data[6]+send_data[i];
    ui->textEdit_show->insertPlainText("停止码 \n");
    data_send();
    qDebug()<<"Finish---data_stop";
}
void serial::data_send_init()
{
    send_data.resize(7);
    send_data[0]=0xff;
    send_data[1]=0x01;
    send_data[2]=0x00;
    send_data[3]=0x00;
    send_data[3]=0x00;
    send_data[4]=0x00;
    send_data[5]=0x00;
    send_data[6]=0x00;
    qDebug()<<"Finish---data_send_init";
}
//串口配置
void serial::on_btn_findport_clicked()
{
    ui->btn_okport->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        ui->btn_okport->addItem(info.portName());
        qDebug()<<info.portName();
    }
}

void serial::on_btn_portstatus_clicked()
{
    if (ui->btn_portstatus->text() == QString("打开串口"))
        {
            /***************串 口 号e*************/
            global_port.setPortName(ui->btn_okport->currentText());
            /***************波 特 率***************/
            global_port.setBaudRate(ui->baudrate->currentText().toInt());
            /***************数 据 位*****************/
            switch (ui->cmb_data->currentIndex())
            {
            case 0:
                global_port.setDataBits(QSerialPort::Data8);
                break;
            case 1:
                global_port.setDataBits(QSerialPort::Data7);
                break;
            case 2:
                global_port.setDataBits(QSerialPort::Data6);
                break;
            default:
                global_port.setDataBits(QSerialPort::Data5);
                break;
            }
            /*************数 据 校 验 位*****************/
            switch (ui->cmb_check->currentIndex())
            {
            case 0:
                global_port.setParity(QSerialPort::NoParity);
                break;
            case 1:
                global_port.setParity(QSerialPort::EvenParity);
                break;
            case 2:
                global_port.setParity(QSerialPort::MarkParity);
                break;
            case 3:
                global_port.setParity(QSerialPort::OddParity);
                break;
            default:
                global_port.setParity(QSerialPort::SpaceParity);
                break;
            }
            /***********停止位*****************/
            switch (ui->cmb_stop->currentIndex())
            {
            case 0:
                global_port.setStopBits(QSerialPort::OneStop);
                break;
            case 1:
                global_port.setStopBits(QSerialPort::OneAndHalfStop);
                break;
            default:
                global_port.setStopBits(QSerialPort::TwoStop);
                break;
            }
            //打开串口
            if (!global_port.open(QIODevice::ReadWrite))
            {
                QMessageBox::about(NULL, "操作结果", "无法打开串口，串口可能不存在或正在使用");
                return;
            }
            else
            {
                qDebug()<<"Finish::open port";
                qDebug()<<global_port.portName();
                qDebug()<<global_port.baudRate();
                qDebug()<<global_port.dataBits();
                ui->btn_okport->setEnabled(false);
                ui->baudrate->setEnabled(false);
                ui->cmb_data->setEnabled(false);
                ui->cmb_check->setEnabled(false);
                ui->cmb_stop->setEnabled(false);
                ui->btn_findport->setEnabled(false);
               // ui->lineEdit_address->setEnabled(false);
                //修改声明
                ui->label_status->setText(QString("已连接!!!"));
                ui->btn_portstatus->setText("关闭串口");
                //发送BUTTON失效
                ui->btn_senddata->setEnabled(true);
                //界面切换TAB开启
                ui->tab_2->setEnabled(true);
                ui->tab_3->setEnabled(true);
                ui->tab_4->setEnabled(true);
            }
        }
        else
        {
            global_port.close();
            //修改声明
            ui->label_status->setText("未连接!!!");
            ui->btn_portstatus->setText("打开串口");
            //发送BUTTON失能
            ui->btn_senddata->setEnabled(false);
            //Button生效
            ui->btn_okport->setEnabled(true);
            ui->baudrate->setEnabled(true);
            ui->cmb_data->setEnabled(true);
            ui->cmb_check->setEnabled(true);
            ui->cmb_stop->setEnabled(true);
            ui->btn_findport->setEnabled(true);
            /*界面切换失效
           ui->tab_2->setEnabled(false);
           ui->tab_3->setEnabled(false);
           ui->tab_4->setEnabled(false);*/
            //QMessageBox::about(NULL, "操作结果", "串口已关闭");
        }
}

//基本控制
void serial::on_btn_leftandup_pressed()
{
    //FF 01 00 0c 0b 0b 23

    data_line=ui->lineEdit_clouspeed->text().toInt();
    data_jiexi=ui->lineEdit_address->text().toInt();
    send_data[1]=char(data_jiexi);
    send_data[2]=0x00;
    send_data[3]=0x0c;
    send_data[4]=char(data_line);
    send_data[5]=char(data_line);
    for(int i=1;i<6;i++)
        send_data[6]=send_data[6]+send_data[i];
    ui->textEdit_show->insertPlainText("左上：\n");
    qDebug()<<"左上:";
    data_send();
}

void serial::on_btn_leftandup_released()
{
    data_stop();
}

void serial::on_btn_up_pressed()
{
    data_line=ui->lineEdit_clouspeed->text().toInt();
    data_jiexi=ui->lineEdit_address->text().toInt();
    send_data[1]=char(data_jiexi);
    send_data[2]=0x00;
    send_data[3]=0x08;
    send_data[4]=0x00;
    send_data[5]=char(data_line);
    for(int i=1;i<6;i++)
        send_data[6]=send_data[6]+send_data[i];
    ui->textEdit_show->insertPlainText("上：\n");
     qDebug()<<"上:";
    data_send();
}

void serial::on_btn_up_released()
{
    data_stop();
}

void serial::on_btn_rightandup_pressed()
{
    data_line=ui->lineEdit_clouspeed->text().toInt();
    data_jiexi=ui->lineEdit_address->text().toInt();
    send_data[1]=char(data_jiexi);
    send_data[2]=0x00;
    send_data[3]=0x0a;
    send_data[4]=char(data_line);
    send_data[5]=char(data_line);
    for(int i=1;i<6;i++)
        send_data[6]=send_data[6]+send_data[i];
    ui->textEdit_show->insertPlainText("右上：\n");
     qDebug()<<"右上:";
    data_send();
}

void serial::on_btn_rightandup_released()
{
    data_stop();
}

void serial::on_btn_left_pressed()
{
    data_line=ui->lineEdit_clouspeed->text().toInt();
    data_jiexi=ui->lineEdit_address->text().toInt();
    send_data[1]=char(data_jiexi);
    send_data[2]=0x00;
    send_data[3]=0x04;
    send_data[4]=char(data_line);
    send_data[5]=0x00;
    for(int i=1;i<6;i++)
        send_data[6]=send_data[6]+send_data[i];
    ui->textEdit_show->insertPlainText("左：\n");
    qDebug()<<"左:";
    data_send();
}

void serial::on_btn_left_released()
{
    data_stop();
}

void serial::on_btn_right_pressed()
{
    data_line=ui->lineEdit_clouspeed->text().toInt();
    data_jiexi=ui->lineEdit_address->text().toInt();
    send_data[1]=char(data_jiexi);
    send_data[2]=0x00;
    send_data[3]=0x02;
    send_data[4]=char(data_line);
    send_data[5]=0x00;
    for(int i=1;i<6;i++)
        send_data[6]=send_data[6]+send_data[i];
    ui->textEdit_show->insertPlainText("右：\n");
    qDebug()<<"右:";
    data_send();
}

void serial::on_btn_right_released()
{
    data_stop();
}

void serial::on_btn_leftanddown_pressed()
{
    data_line=ui->lineEdit_clouspeed->text().toInt();
    data_jiexi=ui->lineEdit_address->text().toInt();
    send_data[1]=char(data_jiexi);
    send_data[2]=0x00;
    send_data[3]=0x14;
    send_data[4]=char(data_line);
    send_data[5]=char(data_line);
    for(int i=1;i<6;i++)
        send_data[6]=send_data[6]+send_data[i];
     qDebug()<<"左下:";
     ui->textEdit_show->insertPlainText("左下：\n");
    data_send();
}

void serial::on_btn_leftanddown_released()
{
    data_stop();
}

void serial::on_btn_down_pressed()
{
    data_line=ui->lineEdit_clouspeed->text().toInt();
    data_jiexi=ui->lineEdit_address->text().toInt();
    send_data[1]=char(data_jiexi);
    send_data[2]=0x00;
    send_data[3]=0x10;
    send_data[4]=0x00;
    send_data[5]=char(data_line);
    for(int i=1;i<6;i++)
        send_data[6]=send_data[6]+send_data[i];
    ui->textEdit_show->insertPlainText("下：\n");
     qDebug()<<"下:";
    data_send();
}

void serial::on_btn_down_released()
{
    data_stop();
}

void serial::on_btn_rightanddown_pressed()
{
    data_line=ui->lineEdit_clouspeed->text().toInt();
    data_jiexi=ui->lineEdit_address->text().toInt();
    send_data[1]=char(data_jiexi);

    send_data[2]=0x00;
    send_data[3]=0x12;
    send_data[4]=char(data_line);
    send_data[5]=char(data_line);
    for(int i=1;i<6;i++)
        send_data[6]=send_data[6]+send_data[i];
    ui->textEdit_show->insertPlainText("右下：\n");
    qDebug()<<"右下:";
    data_send();
}

void serial::on_btn_rightanddown_released()
{
    data_stop();
}
//变倍
void serial::on_btn_bianbei_T_pressed()
{
    data_jiexi=ui->lineEdit_address->text().toInt();
    send_data[1]=char(data_jiexi);
    send_data[2]=0x00;
    send_data[3]=0x20;
    send_data[4]=0x00;
    send_data[5]=0x00;
    for(int i=1;i<6;i++)
        send_data[6]=send_data[6]+send_data[i];
    ui->textEdit_show->insertPlainText("变倍 T：\n");
    data_send();
}

void serial::on_btn_bianbei_T_released()
{
    data_stop();
}

void serial::on_btn_bianbei_W_pressed()
{
    data_jiexi=ui->lineEdit_address->text().toInt();
    send_data[1]=char(data_jiexi);
    send_data[2]=0x00;
    send_data[3]=0x40;
    send_data[4]=0x00;
    send_data[5]=0x00;
    for(int i=1;i<6;i++)
        send_data[6]=send_data[6]+send_data[i];
    ui->textEdit_show->insertPlainText("变倍 W：\n");
    data_send();
}

void serial::on_btn_bianbei_W_released()
{
    data_stop();
}

void serial::on_btn_jujiao_Far_pressed()
{
    data_jiexi=ui->lineEdit_address->text().toInt();
    send_data[1]=char(data_jiexi);
    send_data[2]=0x00;
    send_data[3]=char(0x80);
    send_data[4]=0x00;
    send_data[5]=0x00;
    for(int i=1;i<6;i++)
        send_data[6]=send_data[6]+send_data[i];
    ui->textEdit_show->insertPlainText("聚焦 远：\n");
    data_send();
}

void serial::on_btn_jujiao_Far_released()
{
    data_stop();
}

void serial::on_btn_jujiao_Close_pressed()
{
    data_jiexi=ui->lineEdit_address->text().toInt();
    send_data[1]=char(data_jiexi);
    send_data[2]=0x01;
    send_data[3]=0x00;
    send_data[4]=0x00;
    send_data[5]=0x00;
    for(int i=1;i<6;i++)
        send_data[6]=send_data[6]+send_data[i];
    ui->textEdit_show->insertPlainText("聚焦 近：\n");
    data_send();
}

void serial::on_btn_jujiao_Close_released()
{
    data_stop();
}

void serial::on_btn_light_big_pressed()
{
    data_jiexi=ui->lineEdit_address->text().toInt();
    send_data[1]=char(data_jiexi);
    send_data[2]=0x02;
    send_data[3]=0x00;
    send_data[4]=0x00;
    send_data[5]=0x00;
    for(int i=1;i<6;i++)
        send_data[6]=send_data[6]+send_data[i];
    ui->textEdit_show->insertPlainText("光圈 大：\n");
    data_send();
}

void serial::on_btn_light_big_released()
{
    data_stop();
}

void serial::on_btn_light_small_pressed()
{
    data_jiexi=ui->lineEdit_address->text().toInt();
    send_data[1]=char(data_jiexi);
    send_data[2]=0x04;
    send_data[3]=0x00;
    send_data[4]=0x00;
    send_data[5]=0x00;
    for(int i=1;i<6;i++)
        send_data[6]=send_data[6]+send_data[i];
    ui->textEdit_show->insertPlainText("光圈 小：\n");
    data_send();
}

void serial::on_btn_light_small_released()
{
    data_stop();
}

void serial::on_btn_rain_status_clicked()
{
    if(ui->btn_rain_status->text()==QString("开雨刷"))
    {
        data_jiexi=ui->lineEdit_address->text().toInt();
        send_data[1]=char(data_jiexi);
        send_data[2]=0x00;
        send_data[3]=0x09;
        send_data[4]=0x00;
        send_data[5]=0x01;
        for(int i=1;i<6;i++)
            send_data[6]=send_data[6]+send_data[i];
        ui->textEdit_show->insertPlainText("开雨刷：\n");
        data_send();
   }else {
        data_jiexi=ui->lineEdit_address->text().toInt();
        send_data[1]=char(data_jiexi);
        send_data[2]=0x00;
        send_data[3]=0x0b;
        send_data[4]=0x00;
        send_data[5]=0x01;
        for(int i=1;i<6;i++)
            send_data[6]=send_data[6]+send_data[i];
        ui->textEdit_show->insertPlainText("关雨刷：\n");
        data_send();
   }
}

void serial::on_btn_laser_clicked()
{
    if(ui->btn_laser->text()==QString("开激光")){
        //FF 01 00 09 00 02 0C
        data_jiexi=ui->lineEdit_address->text().toInt();
        send_data[1]=char(data_jiexi);
        send_data[2]=0x00;
        send_data[3]=0x09;
        send_data[4]=0x02;
        send_data[5]=0x00;
        for(int i=1;i<6;i++)
            send_data[6]=send_data[6]+send_data[i];
        ui->textEdit_show->insertPlainText("开激光：\n");
        data_send();
    }else {
        //FF 01 00 0B 00 02 0E
        data_jiexi=ui->lineEdit_address->text().toInt();
        send_data[1]=char(data_jiexi);
        send_data[2]=0x00;
        send_data[3]=0x0b;
        send_data[4]=0x00;
        send_data[5]=0x02;
        for(int i=1;i<6;i++)
            send_data[6]=send_data[6]+send_data[i];
        ui->textEdit_show->insertPlainText("关激光：\n");
        data_send();
    }
}
//声波控制
void serial::on_btn_aco_clicked()
{
    data_jiexi=ui->lineEdit_address->text().toInt();
    data_line=ui->lineEdit_device_id->text().toInt();
    send_data[3]=char(data_line);
    send_data[1]=char(data_jiexi);
    send_data[2]=0x7e;

    send_data[4]=0x15;
    send_data[5]=0x00;
    for(int i=1;i<6;i++)
        send_data[6]=send_data[6]+send_data[i];
    ui->textEdit_show->insertPlainText("声波设备查询：\n");

    data_send();
}

void serial::on_btn_aco_status_clicked()
{
    if(ui->btn_aco_status->text()==QString("打开")){
        data_jiexi=ui->lineEdit_address->text().toInt();
        data_line=ui->lineEdit_device_id->text().toInt();
        send_data[1]=char(data_jiexi);
        send_data[2]=0x7e;
        send_data[3]=char(data_line);
        send_data[4]=0x00;
        send_data[5]=0x01;
        for(int i=1;i<6;i++)
            send_data[6]=send_data[6]+send_data[i];
        ui->textEdit_show->insertPlainText("打开声波设备：\n");
        data_send();
    }else {
        data_jiexi=ui->lineEdit_address->text().toInt();
        data_line=ui->lineEdit_device_id->text().toInt();
        send_data[1]=char(data_jiexi);
        send_data[2]=0x7e;
        send_data[3]=char(data_line);
        send_data[4]=0x00;
        send_data[5]=0x00;
        for(int i=1;i<6;i++)
            send_data[6]=send_data[6]+send_data[i];
        ui->textEdit_show->insertPlainText("关闭声波设备：\n");
        data_send();
    }
}

void serial::on_btn_strongsound_status_clicked()
{

    if(ui->btn_strongsound_status->text()==QString("打开")){
        data_jiexi=ui->lineEdit_address->text().toInt();
        data_line=ui->lineEdit_device_id->text().toInt();
        send_data[1]=char(data_jiexi);
        send_data[2]=0x7e;
        send_data[3]=char(data_line);
        send_data[4]=0x05;
        send_data[5]=0x01;
        for(int i=1;i<6;i++)
            send_data[6]=send_data[6]+send_data[i];
        ui->textEdit_show->insertPlainText("打开强声设备：\n");
        data_send();
    }else {
        data_jiexi=ui->lineEdit_address->text().toInt();
        data_line=ui->lineEdit_device_id->text().toInt();
        send_data[1]=char(data_jiexi);
        send_data[2]=0x7e;
        send_data[3]=char(data_line);
        send_data[4]=0x05;
        send_data[5]=0x02;
        for(int i=1;i<6;i++)
            send_data[6]=send_data[6]+send_data[i];
        ui->textEdit_show->insertPlainText("关闭强声设备：\n");
        data_send();
    }
}

void serial::on_btn_alarm_clicked()
{
    data_jiexi=ui->lineEdit_address->text().toInt();
    data_line=ui->lineEdit_device_id->text().toInt();
    send_data[3]=char(data_line);
    send_data[1]=char(data_jiexi);
    send_data[2]=0x7e;
    send_data[4]=0x11;
    send_data[5]=0x00;
    for(int i=1;i<6;i++)
        send_data[6]=send_data[6]+send_data[i];
    ui->textEdit_show->insertPlainText("警报状态查询：\n");
    data_send();
}

void serial::on_btn_alarm_status_clicked()
{
    if(ui->btn_alarm_status->text()==QString("打开")){
        data_jiexi=ui->lineEdit_address->text().toInt();
        data_line=ui->lineEdit_device_id->text().toInt();
        send_data[3]=char(data_line);
        send_data[1]=char(data_jiexi);
        send_data[2]=0x7e;
        send_data[4]=0x01;
        send_data[5]=0x01;
        for(int i=1;i<6;i++)
            send_data[6]=send_data[6]+send_data[i];
        ui->textEdit_show->insertPlainText("打开警报：\n");
        data_send();
    }else {
        data_jiexi=ui->lineEdit_address->text().toInt();
        data_line=ui->lineEdit_device_id->text().toInt();
        send_data[3]=char(data_line);
        send_data[1]=char(data_jiexi);
        send_data[2]=0x7e;
        send_data[4]=0x01;
        send_data[5]=0x02;
        for(int i=1;i<6;i++)
            send_data[6]=send_data[6]+send_data[i];
        ui->textEdit_show->insertPlainText("关闭警报：\n");
        data_send();
    }
}

void serial::on_btn_gongfang_clicked()
{
    data_jiexi=ui->lineEdit_address->text().toInt();
    data_line=ui->lineEdit_device_id->text().toInt();
    send_data[3]=char(data_line);
    send_data[1]=char(data_jiexi);
    send_data[2]=0x7e;
    send_data[4]=0x05;
    send_data[5]=char(0xdd);
    for(int i=1;i<6;i++)
        send_data[6]=send_data[6]+send_data[i];
    ui->textEdit_show->insertPlainText("功放状态查询：\n");
    data_send();
}

void serial::on_btn_gongfang_status_clicked()
{
    if(ui->btn_gongfang_status->text()==QString("打开")){
        data_jiexi=ui->lineEdit_address->text().toInt();
        data_line=ui->lineEdit_device_id->text().toInt();
        send_data[3]=char(data_line);
        send_data[1]=char(data_jiexi);
        send_data[2]=0x7e;
        send_data[4]=0x05;
        send_data[5]=0x04;
        for(int i=1;i<6;i++)
            send_data[6]=send_data[6]+send_data[i];
        ui->textEdit_show->insertPlainText("打开功放：\n");
        data_send();
    }else {
        data_jiexi=ui->lineEdit_address->text().toInt();
        data_line=ui->lineEdit_device_id->text().toInt();
        send_data[3]=char(data_line);
        send_data[1]=char(data_jiexi);
        send_data[2]=0x7e;
        send_data[4]=0x05;
        send_data[5]=0x03;
        for(int i=1;i<6;i++)
            send_data[6]=send_data[6]+send_data[i];
        ui->textEdit_show->insertPlainText("关闭功放：\n");
        data_send();
    }
}

void serial::on_btn_micro_status_clicked()
{
    if(ui->btn_micro_status->text()==QString("打开")){
        data_jiexi=ui->lineEdit_address->text().toInt();
        data_line=ui->lineEdit_device_id->text().toInt();
        send_data[3]=char(data_line);
        send_data[1]=char(data_jiexi);
        send_data[2]=0x7e;
        send_data[4]=0x06;
        send_data[5]=0x01;
        for(int i=1;i<6;i++)
            send_data[6]=send_data[6]+send_data[i];
        ui->textEdit_show->insertPlainText("打开麦克风：\n");
        data_send();
    }else {
        data_jiexi=ui->lineEdit_address->text().toInt();
        data_line=ui->lineEdit_device_id->text().toInt();
        send_data[3]=char(data_line);
        send_data[1]=char(data_jiexi);
        send_data[2]=0x7e;
        send_data[4]=0x06;
        send_data[5]=0x00;
        for(int i=1;i<6;i++)
            send_data[6]=send_data[6]+send_data[i];
        ui->textEdit_show->insertPlainText("关闭麦克风：\n");
        data_send();
    }
}

void serial::on_btn_recordsound_status_clicked()
{
    if(ui->btn_recordsound_status->text()==QString("打开")){
        data_jiexi=ui->lineEdit_address->text().toInt();
        data_line=ui->lineEdit_device_id->text().toInt();
        send_data[3]=char(data_line);
        send_data[1]=char(data_jiexi);
        send_data[2]=0x7e;
        send_data[4]=0x07;
        send_data[5]=0x01;
        for(int i=1;i<6;i++)
            send_data[6]=send_data[6]+send_data[i];
        ui->textEdit_show->insertPlainText("打开喊话录音：\n");
        data_send();
    }else {
        data_jiexi=ui->lineEdit_address->text().toInt();
        data_line=ui->lineEdit_device_id->text().toInt();
        send_data[3]=char(data_line);
        send_data[1]=char(data_jiexi);
        send_data[2]=0x7e;
        send_data[4]=0x07;
        send_data[5]=0x00;
        for(int i=1;i<6;i++)
            send_data[6]=send_data[6]+send_data[i];
        ui->textEdit_show->insertPlainText("关闭喊话录音：\n");
        data_send();
    }
}
//播放控制

void serial::on_btn_music_go_clicked()
{
    data_jiexi=ui->lineEdit_address->text().toInt();
    data_line=ui->lineEdit_device_id->text().toInt();
    send_data[3]=char(data_line);
    send_data[1]=char(data_jiexi);
    send_data[2]=0x7e;
    send_data[4]=0x02;
    send_data[5]=0x07;
    for(int i=1;i<6;i++)
        send_data[6]=send_data[6]+send_data[i];
    ui->textEdit_show->insertPlainText("快进：\n");
    data_send();
}

void serial::on_btn_music_last_clicked()
{
    data_jiexi=ui->lineEdit_address->text().toInt();
    data_line=ui->lineEdit_device_id->text().toInt();
    send_data[3]=char(data_line);
    send_data[1]=char(data_jiexi);
    send_data[2]=0x7e;
    send_data[4]=0x02;
    send_data[5]=0x01;
    for(int i=1;i<6;i++)
        send_data[6]=send_data[6]+send_data[i];
    ui->textEdit_show->insertPlainText("下一曲：\n");
    data_send();
}

void serial::on_btn_music_status_clicked()
{
    if(ui->btn_music_status->text()==QString("播放")){
        //FF 02 7E 01 02 04 87
        data_jiexi=ui->lineEdit_address->text().toInt();
        data_line=ui->lineEdit_device_id->text().toInt();
        send_data[3]=char(data_line);
        send_data[1]=char(data_jiexi);
        send_data[2]=0x7e;
        send_data[4]=0x02;
        send_data[5]=0x04;
        for(int i=1;i<6;i++)
            send_data[6]=send_data[6]+send_data[i];
        ui->textEdit_show->insertPlainText("播放：\n");
        data_send();
    }else {
        data_jiexi=ui->lineEdit_address->text().toInt();
        data_line=ui->lineEdit_device_id->text().toInt();
        send_data[3]=char(data_line);
        send_data[1]=char(data_jiexi);
        send_data[2]=0x7e;
        send_data[4]=0x02;
        send_data[5]=0x03;
        for(int i=1;i<6;i++)
            send_data[6]=send_data[6]+send_data[i];
        ui->textEdit_show->insertPlainText("停止：\n");
        data_send();
    }
}

void serial::on_btn_music_next_clicked()
{
    data_jiexi=ui->lineEdit_address->text().toInt();
    data_line=ui->lineEdit_device_id->text().toInt();
    send_data[3]=char(data_line);
    send_data[1]=char(data_jiexi);
    send_data[2]=0x7e;
    send_data[4]=0x02;
    send_data[5]=0x02;
    for(int i=1;i<6;i++)
        send_data[6]=send_data[6]+send_data[i];
    ui->textEdit_show->insertPlainText("下一曲：\n");
    data_send();
}

void serial::on_btn_music_back_clicked()
{

    data_jiexi=ui->lineEdit_address->text().toInt();
    data_line=ui->lineEdit_device_id->text().toInt();
    send_data[3]=char(data_line);
    send_data[1]=char(data_jiexi);
    send_data[2]=0x7e;
    send_data[4]=0x02;
    send_data[5]=0x08;
    for(int i=1;i<6;i++)
        send_data[6]=send_data[6]+send_data[i];
    ui->textEdit_show->insertPlainText("快退：\n");
    data_send();
}

void serial::on_btn_music_circle_clicked()
{
    data_jiexi=ui->lineEdit_address->text().toInt();
    data_line=ui->lineEdit_device_id->text().toInt();
    send_data[3]=char(data_line);
    send_data[1]=char(data_jiexi);
    send_data[2]=0x7e;
    send_data[4]=0x12;
    send_data[5]=0x02;
    for(int i=1;i<6;i++)
        send_data[6]=send_data[6]+send_data[i];
    ui->textEdit_show->insertPlainText("循环状态查询：\n");
    data_send();
}

void serial::on_btn_music_change_clicked()
{
    //FF 02 7E 01 02 06 89
    if(ui->lineEdit_music_circle->text()==QString("单曲循环")){
        data_jiexi=ui->lineEdit_address->text().toInt();
        data_line=ui->lineEdit_device_id->text().toInt();
        send_data[3]=char(data_line);
        send_data[1]=char(data_jiexi);
        send_data[2]=0x7e;
        send_data[4]=0x02;
        send_data[5]=0x06;
        for(int i=1;i<6;i++)
            send_data[6]=send_data[6]+send_data[i];
        ui->textEdit_show->insertPlainText("单曲-->列表：\n");
        data_send();
    }else {
       // FF 02 7E 01 02 05 88
        data_jiexi=ui->lineEdit_address->text().toInt();
        data_line=ui->lineEdit_device_id->text().toInt();
        send_data[3]=char(data_line);
        send_data[1]=char(data_jiexi);
        send_data[2]=0x7e;
        send_data[4]=0x02;
        send_data[5]=0x05;
        for(int i=1;i<6;i++)
            send_data[6]=send_data[6]+send_data[i];
        ui->textEdit_show->insertPlainText("列表-->单曲：\n");
        data_send();
    }
}

void serial::on_btn_music_status_check_clicked()
{
    //FF 02 7E 01 12 01 94
    data_jiexi=ui->lineEdit_address->text().toInt();
    data_line=ui->lineEdit_device_id->text().toInt();
    send_data[3]=char(data_line);
    send_data[1]=char(data_jiexi);
    send_data[2]=0x7e;
    send_data[4]=0x12;
    send_data[5]=0x01;
    for(int i=1;i<6;i++)
        send_data[6]=send_data[6]+send_data[i];
    ui->textEdit_show->insertPlainText("播放状态：\n");
    data_send();
}

void serial::on_btn_music_volume_clicked()
{
    //FF 02 7E 01 14 00 95
    data_jiexi=ui->lineEdit_address->text().toInt();
    data_line=ui->lineEdit_device_id->text().toInt();
    send_data[3]=char(data_line);
    send_data[1]=char(data_jiexi);
    send_data[2]=0x7e;
    send_data[4]=0x14;
    send_data[5]=0x00;
    for(int i=1;i<6;i++)
        send_data[6]=send_data[6]+send_data[i];
    ui->textEdit_show->insertPlainText("音量查询：\n");
    data_send();
}

void serial::on_btn_music_voulume_config_clicked()
{
    //FF 02 7E 01 04 05 8A
    data_line=ui->lineEdit_music_volume->text().toInt();
    data_jiexi=ui->lineEdit_address->text().toInt();
    int datavolume=ui->lineEdit_music_volume->text().toInt();
    send_data[1]=char(data_jiexi);
    send_data[2]=0x7e;
    send_data[3]=char(data_line);
    send_data[4]=0x04;
    send_data[5]=char(datavolume);
    for(int i=1;i<6;i++)
        send_data[6]=send_data[6]+send_data[i];
    ui->textEdit_show->insertPlainText("音量设定：\n");
    data_send();
}

//播放曲目
void serial::on_btn_music_count_clicked()
{
    data_jiexi=ui->lineEdit_address->text().toInt();
    data_line=ui->lineEdit_device_id->text().toInt();
    send_data[3]=char(data_line);
    send_data[1]=char(data_jiexi);
    send_data[2]=0x7e;
    send_data[4]=0x16;
    send_data[5]=0x00;
    for(int i=1;i<6;i++)
        send_data[6]=send_data[6]+send_data[i];
    ui->textEdit_show->insertPlainText("播放曲目总数：\n");
    data_send();
}

void serial::on_btn_music_nowno_clicked()
{
    data_jiexi=ui->lineEdit_address->text().toInt();
    data_line=ui->lineEdit_device_id->text().toInt();
    send_data[3]=char(data_line);
    send_data[1]=char(data_jiexi);
    send_data[2]=0x7e;
    send_data[4]=0x13;
    send_data[5]=0x00;
    for(int i=1;i<6;i++)
        send_data[6]=send_data[6]+send_data[i];
    ui->textEdit_show->insertPlainText("当前播放曲目序号：\n");
    data_send();
}

void serial::on_btn_music_startnow_clicked()
{
    //FF 02 7E 01 03 05 89
    data_line=ui->lineEdit_device_id->text().toInt();
    data_jiexi=ui->lineEdit_address->text().toInt();
    send_data[1]=char(data_jiexi);
    send_data[2]=0x7e;
    send_data[3]=char(data_line);
    send_data[4]=0x03;
    send_data[5]=char(ui->lineEdit_nowno->text().toInt());
    for(int i=1;i<6;i++)
        send_data[6]=send_data[6]+send_data[i];
    ui->textEdit_show->insertPlainText("播放当前序号曲：\n");
    data_send();
}

void serial::on_btn_device_check_clicked()
{
    //FF 02 7E 01 18 01 9A
    //FF 02 7E 03 18 01 9C

    data_line=ui->lineEdit_device_id->text().toInt();
    data_jiexi=ui->lineEdit_address->text().toInt();
    send_data[1]=char(data_jiexi);
    send_data[2]=0x7e;
    send_data[3]=char(data_line);
    send_data[4]=0x18;
    send_data[5]=0x01;
    for(int i=1;i<6;i++)
        send_data[6]=send_data[6]+send_data[i];
    ui->textEdit_show->insertPlainText("设备异常查询：\n");
    data_send();
}

//光线控制

void serial::on_btn_white_oepn_clicked()
{
    //FF 03 00 09 00 00 0C
    data_jiexi=ui->lineEdit_address->text().toInt();
    send_data[1]=char(data_jiexi);
    send_data[2]=0x00;
    send_data[3]=0x09;
    send_data[4]=0x00;
    send_data[5]=0x00;
    for(int i=1;i<6;i++)
        send_data[6]=send_data[6]+send_data[i];
    ui->textEdit_show->insertPlainText("开启白光：\n");
    data_send();
}

void serial::on_btn_white_close_clicked()
{
    //FF 03 00 0B 00 01 0F
    data_jiexi=ui->lineEdit_address->text().toInt();
    send_data[1]=char(data_jiexi);
    send_data[2]=0x00;
    send_data[3]=0x0b;
    send_data[4]=0x00;
    send_data[5]=0x01;
    for(int i=1;i<6;i++)
        send_data[6]=send_data[6]+send_data[i];
    ui->textEdit_show->insertPlainText("关闭白光：\n");
    data_send();
}

void serial::on_btn_white_spark_clicked()
{
    //FF 03 00 09 00 01 0D
    //FF 03 00 09 00 04 10
    data_line=ui->lineEdit_white_spark->text().toInt();
    data_jiexi=ui->lineEdit_address->text().toInt();
    send_data[1]=char(data_jiexi);
    send_data[2]=0x00;
    send_data[3]=0x09;
    send_data[4]=0x00;
    send_data[5]=char(data_line);
    for(int i=1;i<6;i++)
        send_data[6]=send_data[6]+send_data[i];
    ui->textEdit_show->insertPlainText("白光闪烁：\n");
    data_send();
}

void serial::on_btn_white_search_clicked()
{
   //FF 03 00 84 00 00 87
    data_jiexi=ui->lineEdit_address->text().toInt();
    send_data[1]=char(data_jiexi);
    send_data[2]=0x00;
    send_data[3]=char(0x84);
    send_data[4]=0x00;
    send_data[5]=0x00;
    for(int i=1;i<6;i++)
        send_data[6]=send_data[6]+send_data[i];
    ui->textEdit_show->insertPlainText("查询白光功率：\n");
    data_send();
}

void serial::on_btn_white_config_clicked()
{
    //FF 03 00 4E 00 0E 5F
    //FF 03 00 4E 00 19 6A
    data_line=ui->lineEdit__white_rate->text().toInt();
    data_jiexi=ui->lineEdit_address->text().toInt();
    send_data[1]=char(data_jiexi);
    send_data[2]=0x00;
    send_data[3]=0x4e;
    send_data[4]=0x00;
    send_data[5]=char(data_line);
    for(int i=1;i<6;i++)
        send_data[6]=send_data[6]+send_data[i];
    ui->textEdit_show->insertPlainText("设置白光功率：\n");
    data_send();
}

void serial::on_btn_red_open_clicked()
{
    //FF 03 01 09 00 00 0D
    data_jiexi=ui->lineEdit_address->text().toInt();
    send_data[1]=char(data_jiexi);
    send_data[2]=0x01;
    send_data[3]=0x09;
    send_data[4]=0x00;
    send_data[5]=0x00;
    for(int i=1;i<6;i++)
        send_data[6]=send_data[6]+send_data[i];
    ui->textEdit_show->insertPlainText("开启红光：\n");
    data_send();
}

void serial::on_btn_red_close_clicked()
{
    //FF 03 01 0B 00 02 11
    data_jiexi=ui->lineEdit_address->text().toInt();
    send_data[1]=char(data_jiexi);
    send_data[2]=0x01;
    send_data[3]=0x0b;
    send_data[4]=0x00;
    send_data[5]=0x02;
    for(int i=1;i<6;i++)
        send_data[6]=send_data[6]+send_data[i];
    ui->textEdit_show->insertPlainText("关闭红光：\n");
    data_send();
}

void serial::on_btn_red_spark_clicked()
{
    //FF 03 01 09 00 01 0E
    //FF 03 01 09 00 09 16
    data_line=ui->lineEdit_red_spark->text().toInt();
    data_jiexi=ui->lineEdit_address->text().toInt();
    send_data[1]=char(data_jiexi);
    send_data[2]=0x01;
    send_data[3]=0x09;
    send_data[4]=0x00;
    send_data[5]=char(data_line);
    for(int i=1;i<6;i++)
        send_data[6]=send_data[6]+send_data[i];
    ui->textEdit_show->insertPlainText("红光闪烁：\n");
    data_send();
}

void serial::on_btn_red_search_clicked()
{
    //FF 03 01 84 00 00 88
    data_jiexi=ui->lineEdit_address->text().toInt();
    send_data[1]=char(data_jiexi);
    send_data[2]=0x01;
    send_data[3]=char(0x84);
    send_data[4]=0x00;
    send_data[5]=0x00;
    for(int i=1;i<6;i++)
        send_data[6]=send_data[6]+send_data[i];
    ui->textEdit_show->insertPlainText("红光功率查询：\n");
    data_send();
}

void serial::on_btn_red_config_clicked()
{
    //FF 03 01 4E 00 01 53
    //FF 03 01 4E 00 11 63
    data_line=ui->lineEdit_red_rate->text().toInt();
    data_jiexi=ui->lineEdit_address->text().toInt();
    send_data[1]=char(data_jiexi);
    send_data[2]=0x01;
    send_data[3]=0x4e;
    send_data[4]=0x00;
    send_data[5]=char(data_line);
    for(int i=1;i<6;i++)
        send_data[6]=send_data[6]+send_data[i];
    ui->textEdit_show->insertPlainText("设置红光功率：\n");
    data_send();
}

void serial::on_btn_green_open_clicked()
{
    //FF 03 02 09 00 00 0E
    data_jiexi=ui->lineEdit_address->text().toInt();
    send_data[1]=char(data_jiexi);
    send_data[2]=0x02;
    send_data[3]=0x09;
    send_data[4]=0x00;
    send_data[5]=0x00;
    for(int i=1;i<6;i++)
        send_data[6]=send_data[6]+send_data[i];
    ui->textEdit_show->insertPlainText("开启绿光：\n");
    data_send();
}

void serial::on_btn_green_close_clicked()
{
    //FF 03 02 0B 00 03 13
    data_jiexi=ui->lineEdit_address->text().toInt();
    send_data[1]=char(data_jiexi);
    send_data[2]=0x02;
    send_data[3]=0x0b;
    send_data[4]=0x00;
    send_data[5]=0x03;
    for(int i=1;i<6;i++)
        send_data[6]=send_data[6]+send_data[i];
    ui->textEdit_show->insertPlainText("开启绿光：\n");
    data_send();
}

void serial::on_btn_green_spark_clicked()
{
    //FF 03 02 09 00 01 0F
    //FF 03 02 09 00 0A 18
    data_line=ui->lineEdit_green_spark->text().toInt();
    data_jiexi=ui->lineEdit_address->text().toInt();
    send_data[1]=char(data_jiexi);
    send_data[2]=0x02;
    send_data[3]=0x09;
    send_data[4]=0x00;
    send_data[5]=char(data_line);
    for(int i=1;i<6;i++)
        send_data[6]=send_data[6]+send_data[i];
    ui->textEdit_show->insertPlainText("绿光闪烁：\n");
    data_send();
}

void serial::on_btn_green_search_clicked()
{
    //FF 03 02 84 00 00 89
    data_jiexi=ui->lineEdit_address->text().toInt();
    send_data[1]=char(data_jiexi);
    send_data[2]=0x02;
    send_data[3]=char(0x84);
    send_data[4]=0x00;
    send_data[5]=0x00;
    for(int i=1;i<6;i++)
        send_data[6]=send_data[6]+send_data[i];
    ui->textEdit_show->insertPlainText("绿光功率查询：\n");
    data_send();
}

void serial::on_btn_green_config_clicked()
{
    //FF 03 02 4E 00 01 54
    //FF 03 02 4E 00 0A 5D
    data_line=ui->lineEdit_green_rate->text().toInt();
    data_jiexi=ui->lineEdit_address->text().toInt();
    send_data[1]=char(data_jiexi);
    send_data[2]=0x02;
    send_data[3]=0x4e;
    send_data[4]=0x00;
    send_data[5]=char(data_line);
    for(int i=1;i<6;i++)
        send_data[6]=send_data[6]+send_data[i];
    ui->textEdit_show->insertPlainText("绿光功率设置：\n");
    data_send();
}

void serial::on_btn_blue_open_clicked()
{
    //FF 03 03 09 00 00 0F
    data_jiexi=ui->lineEdit_address->text().toInt();
    send_data[1]=char(data_jiexi);
    send_data[2]=0x03;
    send_data[3]=0x09;
    send_data[4]=0x00;
    send_data[5]=0x00;
    for(int i=1;i<6;i++)
        send_data[6]=send_data[6]+send_data[i];
    ui->textEdit_show->insertPlainText("开启蓝光：\n");
    data_send();
}

void serial::on_btn_blue_close_clicked()
{
    //FF 03 03 0B 00 04 15
    data_jiexi=ui->lineEdit_address->text().toInt();
    send_data[1]=char(data_jiexi);
    send_data[2]=0x03;
    send_data[3]=0x0b;
    send_data[4]=0x00;
    send_data[5]=0x04;
    for(int i=1;i<6;i++)
        send_data[6]=send_data[6]+send_data[i];
    ui->textEdit_show->insertPlainText("关闭蓝光：\n");
    data_send();
}

void serial::on_btn_blue_spark_clicked()
{
    //FF 03 03 09 00 01 10
    //FF 03 03 09 00 0A 19
    data_line=ui->lineEdit_blue_spark->text().toInt();
    data_jiexi=ui->lineEdit_address->text().toInt();
    send_data[1]=char(data_jiexi);
    send_data[2]=0x03;
    send_data[3]=0x09;
    send_data[4]=0x00;
    send_data[5]=char(data_line);
    for(int i=1;i<6;i++)
        send_data[6]=send_data[6]+send_data[i];
    ui->textEdit_show->insertPlainText("蓝光闪烁：\n");
    data_send();
}

void serial::on_btn_blue_search_clicked()
{
    //FF 03 03 84 00 00 8A
    data_jiexi=ui->lineEdit_address->text().toInt();
    send_data[1]=char(data_jiexi);
    send_data[2]=0x03;
    send_data[3]=char(0x84);
    send_data[4]=0x00;
    send_data[5]=0x00;
    for(int i=1;i<6;i++)
        send_data[6]=send_data[6]+send_data[i];
    ui->textEdit_show->insertPlainText("蓝光功率查询：\n");
    data_send();
}

void serial::on_btn_blue_config_clicked()
{
    //FF 03 03 4E 00 01 55
    //FF 03 03 4E 00 0A 5E
    data_line=ui->lineEdit_blue_rate->text().toInt();
    data_jiexi=ui->lineEdit_address->text().toInt();
    send_data[1]=char(data_jiexi);
    send_data[2]=0x03;
    send_data[3]=0x4e;
    send_data[4]=0x00;
    send_data[5]=char(data_line);
    for(int i=1;i<6;i++)
        send_data[6]=send_data[6]+send_data[i];
    ui->textEdit_show->insertPlainText("蓝光功率设置：\n");
    data_send();
}

void serial::on_btn_laser_big_pressed()
{
    //FF 03 02 00 00 00 05
    data_jiexi=ui->lineEdit_address->text().toInt();
    send_data[1]=char(data_jiexi);
    send_data[2]=0x02;
    send_data[3]=0x00;
    send_data[4]=0x00;
    send_data[5]=0x00;
    for(int i=1;i<6;i++)
        send_data[6]=send_data[6]+send_data[i];
    ui->textEdit_show->insertPlainText("光圈大：\n");
    data_send();
}

void serial::on_btn_laser_big_released()
{

    data_stop();
}

void serial::on_btn_laser_small_pressed()
{
    //FF 03 04 00 00 00 07
    data_jiexi=ui->lineEdit_address->text().toInt();
    send_data[1]=char(data_jiexi);
    send_data[2]=0x04;
    send_data[3]=0x00;
    send_data[4]=0x00;
    send_data[5]=0x00;
    for(int i=1;i<6;i++)
        send_data[6]=send_data[6]+send_data[i];
    ui->textEdit_show->insertPlainText("光圈小：\n");
    data_send();

}

void serial::on_btn_laser_small_released()
{
    data_stop();
}

void serial::on_btn_laser_config_clicked()
{
    //FF 03 00 4F 00 01 53
    //FF 03 00 4F 00 0F 61
    data_line=ui->lineEdit_laser_location->text().toInt();
    data_jiexi=ui->lineEdit_address->text().toInt();
    send_data[1]=char(data_jiexi);
    send_data[2]=0x00;
    send_data[3]=0x4f;
    send_data[4]=0x00;
    send_data[5]=char(data_line);
    for(int i=1;i<6;i++)
        send_data[6]=send_data[6]+send_data[i];
    ui->textEdit_show->insertPlainText("激光位置设置：\n");
    data_send();
}

//test send
void serial::on_btn_senddata_clicked()
{
    data_line=ui->lineEdit_senddata->text().toInt();
    send_data[1]=0x01;
    send_data[2]=0x0a;
    send_data[3]=char(0xf1);
    send_data[4]=0x1f;
    send_data[5]=char(0xff);
    qDebug()<<send_data;
    for(int i=1;i<6;i++)
       // send_data[6]=send_data[6]+send_data[i];
    {
         //qDebug("",QVariant((unsigned char)send_data[i]).toInt(),QVariant((unsigned char)send_data[i]).toInt());

         qDebug()<<QVariant((unsigned char)send_data[i]).toInt();//,QVariant((unsigned char)send_data[i]).toInt();
     }
        //qDebug()<<QVariant(send_data[1]).toInt();
//    data_send();
//    data_stop();


}

void serial::on_btn_clear_clicked()
{
    ui->textEdit_show->clear();
}

void serial::on_btn_mid_clicked()
{
    data_jiexi=ui->lineEdit_address->text().toInt();
    send_data[1]=char(data_jiexi);
    //ff0100 07 00636b

    send_data[2]=0x00;
    send_data[3]=0x07;
    send_data[4]=0x00;
    send_data[5]=0x63;
    qDebug()<<send_data;
    for(int i=1;i<6;i++){
        send_data[6]=send_data[6]+send_data[i];
    }
    data_send();
}

void serial::on_btn_clear_recv_clicked()
{
    recv_data.clear();
}
