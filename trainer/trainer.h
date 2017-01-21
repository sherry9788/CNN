#ifndef TRAINER_H
#define TRAINER_H

#include "util/util.h"
#include "util/timer.h"

#include "layer/conv_layer.h"
#include "layer/fully_connected_layer.h"
#include "layer/input_layer.h"
#include "layer/output_layer.h"
#include "layer/pooling_layer.h"

#include "file_op/loader.h"
#include "file_op/saver.h"

#include "MNIST_DB/loader.h"

#include "network/network.h"
#include "network/tester.h"

class trainer;

void singleTest(trainer &test_trainer);

double bulkTest(trainer test_trainer, int testSize = 10000, bool printPicture = false);

class trainer
{
public:

    trainer(int version);
    trainer();

    void train(int count);

    vector<double> get_output(vector<double> input);

    imageDB TrainImage;
    labelDB TrainLabel;
    imageDB TestImage;
    labelDB TestLabel;

    void single_test(string file_path);

private:

    int m_version;

    network *m_net;
};

trainer::trainer(int version):
    TrainImage("MNIST_DB/TrainImages"),
    TrainLabel("MNIST_DB/TrainLabels"),
    TestImage("MNIST_DB/TestImages"),
    TestLabel("MNIST_DB/TestLabels"),
    m_version(version)
{
    string file_path;
    std::stringstream ss;
    ss << version;
    ss >> file_path;
    ss.clear();

    file_path = "neuron_data_" + file_path;

    m_net = new network(file_path);
}

trainer::trainer():
    TrainImage("MNIST_DB/TrainImages"),
    TrainLabel("MNIST_DB/TrainLabels"),
    TestImage("MNIST_DB/TestImages"),
    TestLabel("MNIST_DB/TestLabels")
{
    m_version = 0;
    m_net = new network();
}

void trainer::train(int count)
{
    for(int i = 0; i < count; ++i)
    {
        for(int seq = 0; seq < 60000; ++ seq)
        {
            UINT8 *train_data = TrainImage[seq];
            vector<double> input;
            for(int pixel = 0; pixel < 784; ++ pixel)
            {
                input.push_back(train_data[pixel] / 256.);
            }

            vector<double> answer(10, 0);
            answer[TrainLabel[seq]] = 1;

            vector<double> output(10, 1);
            timer _timer;
            m_net->input_data(input);
            double error = m_net->output_data(output, answer);
            while(_timer.difference() < 1000000 && error > 0.1)
            {
                m_net->input_data(input);
                error = m_net->output_data(output, answer);
                m_net->epoch();
            }
            /////////////////////////

            if(seq % 100 == 0)
                debug(bulkTest(*this, 1000));
        }
        debug(bulkTest(*this, 10000));
    }
}

vector<double> trainer::get_output(vector<double> input)
{
    m_net->input_data(input);
    return m_net->output_data();
}

void singleTest(trainer &test_trainer)
{
    int seq = 0;
    while(seq > -1)
    {
        std::cin >> seq;
        std::vector<double> input(784, 0);
        UINT8 *pixel = test_trainer.TestImage[seq];
        for(int i = 0; i < 784; ++i)
            input[i] = *(pixel++) / 256.;
        std::vector<double> output = test_trainer.get_output(input);
        int i = 0;
        int second_guess = 0;
        for(int j = 0; j < 10; ++ j)
            if(output[j] > output[i])
                i = j;
        if(second_guess == i)
            second_guess = i - 1;
        if (second_guess == -1)
            second_guess = 2;
        for(int j = 0; j < 10; ++j)
            if(output[second_guess] > output[j]
                    && second_guess != i)
                second_guess = j;
        std::cout << "the best try: " << i << std::endl;
        cout << "the second guess: " << second_guess << endl;
        int answer = (int)test_trainer.TestLabel[seq];
        std::cout << "the answer is " << answer << std::endl;
        if(answer != i)
        {
            cout << "network doesn't work well, do you want to"
                    " print the image in bmp format?" << endl;
            cout << "(y for yes, else for no)" << endl;
            char choice;
            cin >> choice;
            if(choice == 'y' || choice == 'Y')
            {
                test_trainer.TestImage.print(seq);
                cout << "done! check it in your directory!" << endl;
                cout << "look at the picture! can you believe it\'s " <<
                     answer << '?' << endl;
                cout << "i can't either!" << endl;
            }
        }
    }
}

double bulkTest(trainer test_trainer, int testSize, bool printPicture)
{
    std::vector<double> input(784, 0);
    int count = 0;
    for(int i = 0; i < testSize; ++i)
    {
        UINT8 *pixel = test_trainer.TestImage[i];
        for(int j = 0; j < 784; ++j)
            input[j] = *(pixel++) / 256.;
        int seq = 0;
        std::vector<double> output = test_trainer.get_output(input);
        for(int j = 0; j < 10; ++ j)
            if(output[j] > output[seq])
                seq = j;
        if(seq == (int)test_trainer.TestLabel[i]) ++count;
        else if(printPicture) test_trainer.TestImage.print(i);
    }
    return count / (double)testSize;
}

void trainer::single_test(string file_path)
{
    m_net = new network(file_path);
    singleTest(*this);
}

#endif // TRAINER_H

