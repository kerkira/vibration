% SETUP:

%Simulation parameters
timestep = 0.001;

%Driven sinusoid properties
A = 1; %amplitude
f = 5; %frequency
VelocityNoise = 10; %injected noise, in velocity units
fc = 1; %high-pass filter time constant

%Derived quantities
aveConst = exp(-2 * pi * fc * timestep); %'b' for one-pole fir filter
	%See: http://www.earlevel.com/main/2012/12/15/a-one-pole-filter/
omega = 2 * pi * f; %angular frequency

% SIMULATION BEGINS: 

%Create some time
t = 0:timestep:1000;
t = t'; %Charlie works in columnar data

%Generate position trace
x = A * cos( omega * t);

%Compute analytic velocity, add noise
vA = - A * omega * sin(omega * t) + VelocityNoise * randn(size(t));


%Allocate memory for reconstructed signal
xReconstructed = zeros(size(x));
xFilteredReconstructed = zeros(size(x));
xMean = zeros(size(x));

%State variable
xPosition = 0;
filteredXPosition = 0;
runningMean = 0;

%Reconstruction loop, equivalent to the arduino loop.
for ctr = 1:rows(t)

	increment = vA(ctr) * timestep;

	%Eulerian step forward
	xPosition += increment;

	%running filter
	filteredXPosition = filteredXPosition * aveConst + increment;

	%Continuous mean subtraction
	runningMean = runningMean * aveConst + (1-aveConst) * xPosition;
	meanSubtracted = xPosition - runningMean;

	%To do:
	%Implement low-pass filter at ~100Hz
	%Compute proportional output

	%Save it (or output feedback to DACs)
	xReconstructed(ctr) = xPosition;
	xFilteredReconstructed(ctr) = filteredXPosition;
	xMean(ctr) = meanSubtracted;
end

% ANALYSIS: 

%Power spectra
P = psd(t, xReconstructed - mean(xReconstructed));
PF = psd(t, xFilteredReconstructed  - mean(xFilteredReconstructed ));
PM = psd(t, xMean  - mean(xMean ));

%Plot power spectra
loglog(	P(:,1), sqrt(P(:,2)),'1;raw reconstruction;',
	PF(:,1), sqrt(PF(:,2)), '2;running filter;',
	PM(:,1), sqrt(PM(:,2)), '3;continuous mean subtraction;'
	)

pause

%Real-space sanity check
plot(	xReconstructed,'1;raw reconstruction;',
	xFilteredReconstructed ,'2;running filter;',
	xMean, '3;continuous mean subtraction;'
	)

