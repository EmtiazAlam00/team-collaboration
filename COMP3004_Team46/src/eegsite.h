#ifndef EEGSITE_H
#define EEGSITE_H

#include <vector>
#include <random>
#include <cmath>
#include <QDebug>
#include <complex>
#include <fftw3.h>

using namespace std;

class EegSite{
    public:
        EegSite(bool isConnected = false);
        static const double SAMPLING_RATE;
        void getSignal(vector<double>& v, double duration);
        static double calculateDominant(vector<double>& v);
        void deliverTreatment(double treatmentFreq, long duration);
private:
        //Used to determine if this is connected to an actual EEG site
        bool connected;
         //Below are const to define the ranges for the four wave types- ranges taken from the patent (US20100036276A1) and (https://www.medicine.mcgill.ca/physio/vlab/biomed_signals/eeg_n.htm)
        static const double ALPHA_MIN_FREQ;
        static const double ALPHA_MAX_FREQ;
        static const double BETA_MIN_FREQ;
        static const double BETA_MAX_FREQ;
        static const double THETA_MIN_FREQ;
        static const double THETA_MAX_FREQ;
        static const double DELTA_MAX_FREQ;
        static const double DELTA_MIN_FREQ;
        //Duration is the length of time of that the wave form is recorded for
        static const double DURATION;
        //Helper functions for the generate wave form
        static void generateWeights(double& alpha, double& beta, double& theta, double& delta);
        static double generateEEGsignal(double time, double alphaWeight, double betaWeight, double thetaWeight, double deltaWeight);
        static void generateFrequencies(double& alpha, double& beta, double& theta, double& delta);
        void generateWaveform(vector<double>& v, double duration);
        static const double PI;
};


#endif
