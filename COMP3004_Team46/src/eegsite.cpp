#include "eegsite.h"

const double PI = acos(-1);

//TEST DATA GENERATION FUNCTIONS

EegSite::EegSite(bool isConnected): connected(isConnected){}

void EegSite::getSignal(vector<double>& v,  double duration){
    if(connected){
        //This is where the Eeg specific function to recieve a signal would be implemented
    }
    generateWaveform(v, duration);
}

const double EegSite::SAMPLING_RATE = 250;
const double EegSite::ALPHA_MIN_FREQ = 8.0;
const double EegSite::ALPHA_MAX_FREQ = 12.0;
const double EegSite::BETA_MIN_FREQ = 12.0;
const double EegSite::BETA_MAX_FREQ= 35.0;
const double EegSite::THETA_MIN_FREQ= 4.0;
const double EegSite::THETA_MAX_FREQ= 8.0;
const double EegSite::DELTA_MAX_FREQ = 4.0;
const double EegSite::DELTA_MIN_FREQ = 0.5;
const double EegSite::DURATION = 60;

void EegSite::generateWaveform(vector<double>& v, double duration){
    double alphaW, betaW, thetaW, deltaW;
    generateWeights(alphaW, betaW, thetaW, deltaW);

    int numSamples = SAMPLING_RATE * duration;
    for(int i = 0; i< numSamples; ++i){
        double time = i/SAMPLING_RATE;
        double eegVal = generateEEGsignal(time, alphaW, betaW, thetaW, deltaW);
        v.push_back(eegVal);
    }
}

double EegSite::generateEEGsignal(double time, double alphaWeight, double betaWeight, double thetaWeight, double deltaWeight)
{
    double alphaFreq, betaFreq, thetaFreq, deltaFreq;
    generateFrequencies(alphaFreq, betaFreq, thetaFreq, deltaFreq);

    double alpha = sin(2.0* M_PI * alphaFreq * time);
    double beta = sin(2.0* M_PI * betaFreq * time);
    double theta = sin(2.0* M_PI * thetaFreq * time);
    double delta = sin(2.0* M_PI * deltaFreq * time);

    double eegSignal = (alphaWeight* alpha)+ (betaWeight* beta)+ (thetaWeight* theta)+ (deltaWeight* delta);
    return eegSignal;
}

void EegSite::generateFrequencies(double &alpha, double &beta, double &theta, double &delta)
{
    random_device rd;
    //Ensures a different number each time
    mt19937 gen(rd());
    //Ensures all the weights are between respective ranges
    uniform_real_distribution<double> disAlpha(ALPHA_MIN_FREQ, ALPHA_MAX_FREQ);
    uniform_real_distribution<double> disBeta(BETA_MIN_FREQ, BETA_MAX_FREQ);
    uniform_real_distribution<double> disTheta(THETA_MIN_FREQ, THETA_MAX_FREQ);
    uniform_real_distribution<double> disDelta(DELTA_MIN_FREQ, DELTA_MAX_FREQ);

    alpha = disAlpha(gen);
    beta = disBeta(gen);
    theta = disTheta(gen);
    delta = disDelta(gen);

}

//Helper function to generate the weights of the different wave types in the eeg signal
void EegSite::generateWeights(double &alpha, double &beta, double &theta, double &delta)
{
    random_device rd;
    //Ensures a different number each time
    mt19937 gen(rd());
    //Ensures all the weights are between 0 and 1
    uniform_real_distribution<double> dis(0.0,1.0);


    alpha = dis(gen);
    beta = dis(gen);
    theta = dis(gen);
    delta = dis(gen);
}


float EegSite::calculateDominant(vector<double>& v)
{
    int N = v.size();
    //Vector of complex doubles as defined in the ffftw library
    vector<fftw_complex> out(N/2+1);

//    qDebug()<<"FFT: "<<Qt::endl;
    fftw_plan plan_forward = fftw_plan_dft_r2c_1d(N, v.data(), reinterpret_cast<fftw_complex*>(out.data()), FFTW_ESTIMATE);

    fftw_execute(plan_forward);

    // Calculate power spectrum
//    qDebug()<< "Power spectrum: "<< Qt::endl;
    vector<double> powerSpectrum(out.size());

    for (size_t k = 0; k < out.size(); ++k) {
        // Calculate the magnitude squared of each complex number
        double magnitudeSquared = out[k][0] * out[k][0] + out[k][1] * out[k][1];
        // Store the magnitude squared in the powerSpectrum vector
        powerSpectrum[k] = magnitudeSquared;
    }

    // Define frequency bands
    struct FrequencyBand {
        double minFreq;
        double maxFreq;
    };

    FrequencyBand alphaBand = {ALPHA_MIN_FREQ, ALPHA_MAX_FREQ};
    FrequencyBand betaBand = {BETA_MIN_FREQ, BETA_MAX_FREQ};
    FrequencyBand thetaBand = {THETA_MIN_FREQ, THETA_MAX_FREQ};
    FrequencyBand deltaBand = {DELTA_MIN_FREQ, DELTA_MAX_FREQ};

    // Calculate total power within each frequency band
    double alphaPower = 0.0, betaPower = 0.0, thetaPower = 0.0, deltaPower = 0.0;
    for (size_t i = 0; i < out.size(); ++i) {
        double freq = i * (SAMPLING_RATE / N);
        double power = powerSpectrum[i];

        if (freq >= alphaBand.minFreq && freq <= alphaBand.maxFreq)
            alphaPower += power;
        else if (freq >= betaBand.minFreq && freq <= betaBand.maxFreq)
            betaPower += power;
        else if (freq >= thetaBand.minFreq && freq <= thetaBand.maxFreq)
            thetaPower += power;
        else if (freq >= deltaBand.minFreq && freq <= deltaBand.maxFreq)
            deltaPower += power;
    }

    // Determine the band with the highest power
    double maxPower = max({alphaPower, betaPower, thetaPower, deltaPower});
    FrequencyBand dominantBand;
    if (maxPower == alphaPower){
//        qInfo()<< "Alpha is dominant";
        dominantBand = alphaBand;
    }
    else if (maxPower == betaPower){
        dominantBand = betaBand;
//        qInfo()<< "Beta is dominant";
    }
    else if (maxPower == thetaPower){
        dominantBand = thetaBand;
//        qInfo()<< "Theta is dominant";
    }
    else{
        dominantBand = deltaBand;
//        qInfo()<< "Delta is dominant";
    }

    // Find the frequency with the maximum power within the dominant band
    double maxFreq = 0.0;
    double maxPowerWithinBand = 0.0;
    for (size_t i = 0; i < out.size(); ++i) {
        double freq = i * (SAMPLING_RATE / N);
        double power = powerSpectrum[i];

        if (freq >= dominantBand.minFreq && freq <= dominantBand.maxFreq && power > maxPowerWithinBand) {
            maxFreq = freq;
            maxPowerWithinBand = power;
        }
    }

    // Destroy the FFT plan
    fftw_destroy_plan(plan_forward);

    return (float)maxFreq;
}

void EegSite::deliverTreatment(double treatmentFreq, long duration){
    qDebug()<< "Delivering treatment of " <<treatmentFreq<< Qt::endl;
}
