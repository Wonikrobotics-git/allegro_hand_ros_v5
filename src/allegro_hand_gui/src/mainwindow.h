#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <ros/ros.h>
#include "std_msgs/Float32.h"
#include "std_msgs/String.h"
#include <ros/package.h>
#include <yaml-cpp/yaml.h>
#include <fstream>
#include <QListWidget>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void savePose(const std::string& pose_file);
    std::vector<double> readFinalJointStates();

    void ChangeButton();

    void TimeChanged(double arg1);

    void on_label_4_linkActivated(const QString &link);

    void ForceValue(double arg1);
    
    void ForceApply();

    void PdNumChanged(int arg1);

    void HomeButton();

    void GraspButton();

    void GravityButton();

    void TorqueoffButton();

    void SaveNumChanged(int arg1);

    void SaveButton();

    void MoveButton();

    void ResetButton();

    void ClearlogButton();

    void ExitButton();

    void SliderPressed();

    void SliderReleased();

    void SliderValueChanged(int value);

    void SavefileButton();

    void LoadfileButton();

    void RefreshListButton();

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_poseCountButton_clicked();

    void on_selectPoseButton_clicked();

    void on_startSequenceButton_clicked();

    void on_refreshButton_clicked();

    void executeSequence();

private:
    Ui::MainWindow *ui;

    int currentSequenceIndex;
    QTimer *sequenceTimer;
    QStringList selectedPoses;
    int repeatCount;
    int executedCycles;  // 추가된 변수
    int poseCount;

    ros::NodeHandle nh_;
    ros::Publisher time_pub_;
    ros::Publisher force_pub_;
    ros::Publisher joint_cmd_pub;
    bool sliderPressed_;

    void listYamlFiles();

    static QString lastSelectedPose; // 마지막 선택된 포즈를 저장하는 변수
    bool selectionCompleteLogged;
};
#endif // MAINWINDOW_H
