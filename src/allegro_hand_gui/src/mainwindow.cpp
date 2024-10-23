#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QFileInfoList>
#include <QListWidgetItem>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    time_pub_ = nh_.advertise<std_msgs::Float32>("timechange", 10);
    force_pub_ = nh_.advertise<std_msgs::Float32>("forcechange", 10);
    joint_cmd_pub = nh_.advertise<std_msgs::String>("/allegroHand_0/lib_cmd", 10);

    ui->listWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);


    sequenceTimer = new QTimer(this);
    connect(sequenceTimer, &QTimer::timeout, this, &MainWindow::executeSequence);

    connect(ui->listWidget, &QListWidget::itemClicked, this, &MainWindow::on_listWidget_itemClicked);

    /// Time Panel
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::ChangeButton);
    connect(ui->doubleSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::TimeChanged);

    /// Grasping force Panel(Button)
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::ForceApply);
    connect(ui->doubleSpinBox_2, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::ForceValue);

    /// Grasping force Panel(Slider)
    connect(ui->horizontalSlider, &QAbstractSlider::sliderReleased, this, &MainWindow::SliderReleased); 
    connect(ui->horizontalSlider, &QAbstractSlider::sliderPressed, this, &MainWindow::SliderPressed);
    connect(ui->horizontalSlider, &QAbstractSlider::valueChanged, this, &MainWindow::SliderValueChanged);

    /// Pre-defined Hand Pose Panel
    connect(ui->pushButton_3, &QPushButton::clicked, this, &MainWindow::HomeButton);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &MainWindow::GraspButton);
    connect(ui->pushButton_5, &QPushButton::clicked, this, &MainWindow::GravityButton);
    connect(ui->pushButton_6, &QPushButton::clicked, this, &MainWindow::TorqueoffButton);

    /// Custom Hand Pose Panel
    connect(ui->spinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::PdNumChanged);
    connect(ui->pushButton_7, &QPushButton::clicked, this, &MainWindow::MoveButton);

    /// FingerTip Sensor Panel
    connect(ui->pushButton_9, &QPushButton::clicked, this, &MainWindow::ResetButton);

    /// Save pose Panel
    connect(ui->spinBox_2, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::SaveNumChanged);
    connect(ui->pushButton_8, &QPushButton::clicked, this, &MainWindow::SaveButton);
    connect(ui->pushButton_12, &QPushButton::clicked, this, &MainWindow::SavefileButton);  


    ////Load pose Panel
    connect(ui->LoadButton, &QPushButton::clicked, this, &MainWindow::LoadfileButton);
    connect(ui->RefreshButton, &QPushButton::clicked, this, &MainWindow::RefreshListButton);        

    /// Else
    connect(ui->pushButton_10, &QPushButton::clicked, this, &MainWindow::ClearlogButton);
    connect(ui->pushButton_11, &QPushButton::clicked, this, &MainWindow::ExitButton);



    connect(ui->poseCountButton, &QPushButton::clicked, this, &MainWindow::on_poseCountButton_clicked);
    connect(ui->selectPoseButton, SIGNAL(clicked()), this, SLOT(on_selectPoseButton_clicked()));

    connect(ui->startSequenceButton, &QPushButton::clicked, this, &MainWindow::on_startSequenceButton_clicked);
    connect(ui->refreshButton, &QPushButton::clicked, this, &MainWindow::on_refreshButton_clicked);   

    ui->poseListWidget->hide();
    ui->selectPoseButton->hide();
    ui->repeatCountSpinBox->hide();
    ui->startSequenceButton->hide();
    ui->label_15->hide();

    currentSequenceIndex = 0;
    repeatCount = 0;
    executedCycles = 0;
    poseCount = 0;
    selectionCompleteLogged = false;

    listYamlFiles();
}

MainWindow::~MainWindow()
{
    delete ui;
}

std::vector<double> MainWindow::readFinalJointStates()
{

  std::string pkg_path = ros::package::getPath("allegro_hand_controllers");
  std::string file_path = pkg_path + "/pose/pose_moveit.yaml";

  YAML::Node node = YAML::LoadFile(file_path);
  std::vector<double> positions = node["position"].as<std::vector<double>>();
  return positions;
}

void MainWindow::savePose(const std::string& pose_file)
{
  std::vector<double> positions = readFinalJointStates();

  YAML::Emitter out;
  out << YAML::BeginMap;
  out << YAML::Key << "position" << YAML::Value << positions;
  out << YAML::EndMap;

  std::string pkg_path = ros::package::getPath("allegro_hand_controllers");
  std::string file_path = pkg_path + "/pose/" + pose_file;

  std::ofstream fout(file_path);

if (!fout) {
    QString logMessage = QString("Failed to open file for writing: %1").arg(QString::fromStdString(file_path));
    ui->logTextEdit->append(logMessage);
    return;
}

  fout << out.c_str();
  fout.close();

  QString logMessage = QString("Pose saved to %1").arg(QString::fromStdString(pose_file));
  ui->logTextEdit->append(logMessage);
}

void MainWindow::SliderValueChanged(int value)
{
    double double_value = (double)(value/10.0);
    ui->doubleSpinBox_2->setValue(double_value);
    if(sliderPressed_){

        QString logMessage = QString("Slider value changed to %1").arg(double_value);
        ui->logTextEdit->append(logMessage);
    
        std_msgs::Float32 msg;
        msg.data = double_value;
        force_pub_.publish(msg);
    }
    
}

void MainWindow::SliderPressed()
{
    sliderPressed_ = true;
}

void MainWindow::SliderReleased()
{
    sliderPressed_ = false;
}

void MainWindow::ChangeButton()
{
    double value = ui->doubleSpinBox->value();
    QString logMessage = QString("Time changed to %1").arg(value);

    ui->logTextEdit->append(logMessage);


    std_msgs::Float32 msg;
    msg.data = value;
    time_pub_.publish(msg);
    
}

void MainWindow::TimeChanged(double arg1)
{

}

void MainWindow::on_label_4_linkActivated(const QString &link)
{

}

void MainWindow::ForceValue(double arg1)
{
    int sliderValue = static_cast<int>(arg1 * 10); // Assuming slider values range in integers
    ui->horizontalSlider->setValue(sliderValue);
}

void MainWindow::ForceApply()
{
    double value = ui->doubleSpinBox_2->value();
    QString logMessage = QString("Force Changed to %1").arg(value);

    ui->logTextEdit->append(logMessage);

    std_msgs::Float32 msg;
    msg.data = value;
    force_pub_.publish(msg);
}


void MainWindow::HomeButton()
{

    std_msgs::String msg;
    std::stringstream ss;

    ss<<"home";

    msg.data = ss.str();
    joint_cmd_pub.publish(msg);

    QString logMessage = QString("Home Position");

    ui->logTextEdit->append(logMessage);

}

void MainWindow::GraspButton()
{

    std_msgs::String msg;
    std::stringstream ss;

    ss<<"grasp_4";

    msg.data = ss.str();
    joint_cmd_pub.publish(msg);

    QString logMessage = QString("Grasp");

    ui->logTextEdit->append(logMessage);

}

void MainWindow::GravityButton()
{

    std_msgs::String msg;
    std::stringstream ss;

    ss<<"gravcomp";

    msg.data = ss.str();
    joint_cmd_pub.publish(msg);

    QString logMessage = QString("Gravity Compensation");

    ui->logTextEdit->append(logMessage);

}

void MainWindow::TorqueoffButton()
{

    std_msgs::String msg;
    std::stringstream ss;

    ss<<"off";

    msg.data = ss.str();
    joint_cmd_pub.publish(msg);

    QString logMessage = QString("Torque off");

    ui->logTextEdit->append(logMessage);

}

void MainWindow::SaveNumChanged(int arg1)
{
    
}

void MainWindow::SaveButton()
{
    int save_num_ = ui->spinBox_2->value();
    std::string pose_file = "pose" + std::to_string(save_num_) + ".yaml";
    savePose(pose_file);

    QString logMessage = QString("Pose saved to %1").arg(QString::fromStdString(pose_file));

    ui->logTextEdit->append(logMessage);

    listYamlFiles();

}

void MainWindow::PdNumChanged(int arg1)
{
    
}

void MainWindow::MoveButton()
{
    int value = ui->spinBox->value();
    QString logMessage = QString("Select Pose Num %1").arg(value);

    ui->logTextEdit->append(logMessage);

    std_msgs::String msg;
    std::stringstream ss;

    ss<<"pdControl"<<value;

    msg.data = ss.str();
    joint_cmd_pub.publish(msg);

}

void MainWindow::ResetButton()
{

    std_msgs::String msg;
    std::stringstream ss;

    ss<<"sensor";

    msg.data = ss.str();
    joint_cmd_pub.publish(msg);

    QString logMessage = QString("Fingertip Sensor Reset");

    ui->logTextEdit->append(logMessage);

}

void MainWindow::ClearlogButton()
{
    ui->logTextEdit->clear();
}

void MainWindow::SavefileButton()
{
    QString fileName = ui->savefilename->text(); // QLineEdit에서 파일명 가져오기

    if (fileName.isEmpty()) {
        QMessageBox::information(this, tr("Input Error"), tr("Please enter a file name."));
        return;
    }

    savePose(fileName.toStdString() + ".yaml");

    listYamlFiles();

}

void MainWindow::LoadfileButton()
{
    QListWidgetItem *selectedItem = ui->listWidget->currentItem();

    if (selectedItem) {
        // 선택된 항목의 이름을 가져와서 로그에 출력
        QString fileName = selectedItem->text();

        std_msgs::String msg;
        std::stringstream ss;

        ss<<fileName.toStdString();
        msg.data = ss.str();
        joint_cmd_pub.publish(msg);

        QString logMessage = QString("Selected item: %1").arg(fileName);
        ui->logTextEdit->append(logMessage);
    } else {
        QString logMessage = QString("No item selected");
        ui->logTextEdit->append(logMessage);
    }
}

void MainWindow::RefreshListButton()
{
    listYamlFiles();
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    QString fileName = item->text();
    // 여기서 파일 로드 등의 동작을 추가할 수 있습니다.
}


void MainWindow::listYamlFiles()
{
    ui->listWidget->clear();

    std::string pkg_path = ros::package::getPath("allegro_hand_controllers");
    QString directoryPath = QString::fromStdString(pkg_path + "/pose/");
    QDir directory(directoryPath);

    // .yaml 파일만 나열
    QStringList yamlFiles = directory.entryList(QStringList() << "*.yaml", QDir::Files);
    foreach (QString filename, yamlFiles) {
        // .yaml 확장자를 제거
        QString displayName = filename;
        displayName.chop(5); // .yaml의 길이인 5를 제거

        QListWidgetItem *item = new QListWidgetItem(displayName);
        ui->listWidget->addItem(item);
    }

}

void MainWindow::on_poseCountButton_clicked()
{
    poseCount = ui->poseCountSpinBox->value();

    ui->poseListWidget->show();
    ui->selectPoseButton->show();

    ui->poseListWidget->clear();

    std::string pkg_path = ros::package::getPath("allegro_hand_controllers");
    QString directoryPath = QString::fromStdString(pkg_path + "/pose/");
    QDir directory(directoryPath);

    // .yaml 파일만 나열
    QStringList yamlFiles = directory.entryList(QStringList() << "*.yaml", QDir::Files);
    foreach (QString filename, yamlFiles) {
        // .yaml 확장자를 제거
        QString displayName = filename;
        displayName.chop(5); // .yaml의 길이인 5를 제거

        QListWidgetItem *item = new QListWidgetItem(displayName);
        ui->poseListWidget->addItem(item);
    }
}

void MainWindow::on_selectPoseButton_clicked()
{

    static QString lastSelectedPose;

    // Check if the current selection is less than the required pose count
    if (selectedPoses.size() < poseCount) {
        // Get the currently selected item from the list widget
        QListWidgetItem *selectedItem = ui->poseListWidget->currentItem();
        
        // Check if an item is selected
        if (selectedItem) {
            QString selectedPose = selectedItem->text(); // Get the text of the selected item

            // Avoid selecting the same pose consecutively
            if (!selectedPoses.isEmpty() && selectedPose == lastSelectedPose) {
            } else {
                // Add the selected pose to the list if it's not already selected
                    selectedPoses.append(selectedPose); // Add to the selected poses list
                    ui->logTextEdit->append("Selected pose: " + selectedPose);

                // Update the last selected pose
                lastSelectedPose = selectedPose;
            }
        }

    }

    // Check if the selection is complete
    if (selectedPoses.size() == poseCount && !selectionCompleteLogged) {
        ui->logTextEdit->append("Selection complete");
        selectionCompleteLogged = true;

        // Show additional UI elements and disable further selection
        ui->repeatCountSpinBox->show();
        ui->startSequenceButton->show();
        ui->refreshButton->show();
        ui->selectPoseButton->setEnabled(false); // Disable the select button
        ui->label_15->show();
    }
}

void MainWindow::on_startSequenceButton_clicked()
{
    double time = ui->doubleSpinBox->value();
    int handtime = (int)(time * 1000) + 200;
    repeatCount = ui->repeatCountSpinBox->value() - 1; // 반복 횟수 가져오기
    currentSequenceIndex = 0;
    ui->logTextEdit->append("Starting sequence...");
    sequenceTimer->start(handtime); // 1초 간격으로 타이머 시작
}

void MainWindow::on_refreshButton_clicked()
{
    ui->poseCountSpinBox->setValue(1); // 포즈 개수를 기본값으로 리셋
    ui->poseListWidget->hide(); // 포즈 리스트 숨기기
    ui->poseListWidget->clear(); // 포즈 리스트 클리어
    ui->selectPoseButton->hide(); // 선택 버튼 숨기기
    ui->repeatCountSpinBox->hide(); // 반복 횟수 스핀박스 숨기기
    ui->repeatCountSpinBox->setValue(1); // 반복 횟수 스핀박스를 기본값으로 리셋
    ui->startSequenceButton->hide(); // 시작 버튼 숨기기
    ui->refreshButton->show(); // 새로고침 버튼 숨기기
    selectedPoses.clear(); // 선택된 포즈 리스트 초기화
    ui->logTextEdit->clear(); // 로그 클리어
    ui->label_15->hide();
    currentSequenceIndex = 0; // 현재 시퀀스 인덱스 초기화
    repeatCount = 0; // 반복 횟수 초기화
    sequenceTimer->stop(); // 타이머 멈추기
    ui->logTextEdit->append("Reset complete. Please select poses again.");
    ui->selectPoseButton->setEnabled(true);
    selectionCompleteLogged = false;
}

void MainWindow::executeSequence()
{
    if (currentSequenceIndex < selectedPoses.size()) {
        QString pose = selectedPoses[currentSequenceIndex];
        std_msgs::String msg;
        msg.data = pose.toStdString();
        joint_cmd_pub.publish(msg);

        ui->logTextEdit->append("Executing pose: " + pose);
        ++currentSequenceIndex;
    } else {
        if (repeatCount > 0) {
            currentSequenceIndex = 0; // 처음 포즈로 돌아감
            --repeatCount; // 반복 횟수 감소
        } else {
            sequenceTimer->stop(); // 반복 횟수 완료 시 타이머 정지
            ui->logTextEdit->append("Sequence completed.");
        }
    }
}


void MainWindow::ExitButton()
{
    TorqueoffButton();
    // ROS 노드를 종료합니다.
    ros::shutdown();
    
    // Qt 애플리케이션을 종료합니다.
    QApplication::quit();

}