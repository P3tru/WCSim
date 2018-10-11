from HKWCSimMacro import HKWCSimMacro

class HKODWCSimMacro(HKWCSimMacro):

    def createGeometry(self):
        # HyperK
        self.macro += '/WCSim/WCgeom HyperKWithOD\n'

        # Vary OD Dimensions and PMTs number
        PMTODRadius = 3
        ODLateralWaterDepth = 1
        ODHeightWaterDepth = 2
        ODDeadSpace = 600
        ODTyvekSheetThickness = 2
        ODWLSPlatesThickness = 1
        PMTODperCellHorizontal = 1
        PMTODperCellVertical = 1
        PMTODPercentCoverage = 1
        ODPMTShift = 0

        self.macro += '/WCSim/HyperKOD/PMTODRadius {0}inch\n'.format(PMTODRadius)
        self.macro += '/WCSim/HyperKOD/ODLateralWaterDepth {0} {1}\n'.format(ODLateralWaterDepth, 'm')
        self.macro += '/WCSim/HyperKOD/ODHeightWaterDepth {0} {1}\n'.format(ODHeightWaterDepth, 'm')
        self.macro += '/WCSim/HyperKOD/ODDeadSpace {0} {1}\n'.format(ODDeadSpace, 'mm')
        self.macro += '/WCSim/HyperKOD/ODTyvekSheetThickness {0} {1}\n'.format(ODTyvekSheetThickness, 'mm')
        self.macro += '/WCSim/HyperKOD/ODWLSPlatesThickness {0} {1}\n'.format(ODWLSPlatesThickness, 'cm')
        self.macro += '/WCSim/HyperKOD/PMTODperCellHorizontal {0}\n'.format(PMTODperCellHorizontal)
        self.macro += '/WCSim/HyperKOD/PMTODperCellVertical {0}\n'.format(PMTODperCellVertical)
        self.macro += '/WCSim/HyperKOD/PMTODPercentCoverage {0}\n'.format(PMTODPercentCoverage)
        self.macro += '/WCSim/HyperKOD/ODPMTShift {0} {1}\n'.format(ODPMTShift, 'm')

        # Update geom
        self.macro += '/WCSim/Construct\n'


    def createODDAQOptions(self):
        # Dark Rate Tank
        DarkRateDetEl = '/DarkRate/SetDetectorElement OD\n'
        DarkRateMode = '/DarkRate/SetDarkMode 1\n'
        DarkRateLow = '/DarkRate/SetDarkLow 0\n'
        DarkRateHigh = '/DarkRate/SetDarkHigh 100000\n'
        DarkRateWindow = '/DarkRate/SetDarkWindow 4000\n'
        self.macro += DarkRateDetEl
        self.macro += DarkRateMode
        self.macro += DarkRateLow
        self.macro += DarkRateHigh
        self.macro += DarkRateWindow

