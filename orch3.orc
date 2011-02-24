@modulator: { osc level=".55" freq="220" }

@carrier: { osc level="0.6" freq=@modulator }

> @carrier / { fileout }


