#ifndef EEGSITE_H
#define EEGSITE_H

#include <vector>
#include <random>
#include <cmath>
#include <QDebug>
#include <complex>
#include <fftw3.h>

using namespace std;
/* Class Purpose: Creates simulated waveforms for testing purposes and calculates the dominant frequency based on an inserted waveform.
 *
 * Data Members:
 * - bool connected: Is set to true if it is connected to an actual EEG- so for simulation and testing purposes the default is false
 * - const PI value - has value of pi for the waveform calculations
 * - const static bounds (e.g. BETA_MIN_FREQ and BETA_MAX_FREQ) to set upper and lower bounds for the different bands (theta, beta, alpha and delta)
 * - static SAMPLING_RATE: the sampling rate of the EEG- set to 250 as that is a very common sampling rate for eegs
 * Class Functions:
 * - void getSignal(): gets the waveform from the EEG- if connected to an actual EEG this would be connected to the function to get the waveform directly otherwise it creates and generates the test waveform
 * - static float calculateDominant(vector<double>& v): Calculates the dominant frequency by doing a Fast Fourier Transform, and a power spectrum analysis
 * - deliverTreatment(double treatmentFreq, long duration): if connected to an actual EEG it would call a function to deliver the treatment frequency for the specified duration, currently just a dummy function and prints that it delivers treatment to the console
 * - generateWaveforms(): simulates waveforms by summing sin waves for the different freqeuncy bands- the different bands have different weights (randomly generated) and random frequency for the different bands
 * - generateEEGSignal(): generates a single signal, several of these signals are used to build the wave form- serves as a helper function
 * - generateWeights and generateFrequencies are helper functions to randomly generate the weights and frequencies for each band
 * - 
 */



class EegSite{
    public:
        EegSite(bool isConnected = false);
        static const double SAMPLING_RATE;
        void getSignal(vector<double>& v, double duration);
        static float calculateDominant(vector<double>& v);
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
