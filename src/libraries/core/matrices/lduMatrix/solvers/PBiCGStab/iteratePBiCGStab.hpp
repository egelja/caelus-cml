        scalarField Aw(nCells);
        scalar* __restrict__ AwPtr = Aw.begin();

        scalarField sA(nCells);
        scalar* __restrict__ sAPtr = sA.begin();

        scalarField zA(nCells);
        scalar* __restrict__ zAPtr = zA.begin();

        scalarField tA(nCells);
        scalar* __restrict__ tAPtr = tA.begin();

        // --- Store initial residual
        const scalarField rA0(rA);

        // --- Initial values not used
        scalar rA0rA = 0;
        scalar alpha = 0;
        scalar omega = 0;

        // --- Select and construct the preconditioner
        autoPtr<lduMatrix::preconditioner> preconPtr =
        lduMatrix::preconditioner::New
        (
            *this,
            controlDict_
        );

        // --- Solver iteration
        do
        {
            // --- Store previous rA0rA
            const scalar rA0rAold = rA0rA;

            rA0rA = gSumProd(rA0, rA);

            // --- Test for singularity
            if (solverPerf.checkSingularity(mag(rA0rA)))
            {
                break;
            }

            // --- Update pA
            if (solverPerf.nIterations() == 0)
            {
                for (label cell=0; cell<nCells; cell++)
                {
                    pAPtr[cell] = rAPtr[cell];
                }
            }
            else
            {
                // --- Test for singularity
                if (solverPerf.checkSingularity(mag(omega)))
                {
                    break;
                }

                const scalar beta = (rA0rA/rA0rAold)*(alpha/omega);

                for (label cell=0; cell<nCells; cell++)
                {
                    pAPtr[cell] =
                        rAPtr[cell] + beta*(pAPtr[cell] - omega*AwPtr[cell]);
                }
            }

            // --- Precondition pA
            preconPtr->precondition(w, pA, cmpt);

            // --- Calculate Aw
            matrix_.Amul(Aw, w, interfaceBouCoeffs_, interfaces_, cmpt);

            const scalar rA0Aw = gSumProd(rA0, Aw);

            alpha = rA0rA/rA0Aw;

            // --- Calculate sA
            for (label cell=0; cell<nCells; cell++)
            {
                sAPtr[cell] = rAPtr[cell] - alpha*AwPtr[cell];
            }

            // --- Test sA for convergence
            solverPerf.finalResidual() = gSumMag(sA)/normFactor;

            if (solverPerf.checkConvergence
                (
                    tolerance_, 
                    relTol_, 
                    solverPerf.nIterations(), 
                    minIter_
                )
               )
            {
                for (label cell=0; cell<nCells; cell++)
                {
                    xPtr[cell] += alpha*wPtr[cell];
                }

                solverPerf.nIterations()++;

                return solverPerf;
            }

            // --- Precondition sA
            preconPtr->precondition(zA, sA, cmpt);

            // --- Calculate tA
            matrix_.Amul(tA, zA, interfaceBouCoeffs_, interfaces_, cmpt);

            const scalar tAtA = gSumSqr(tA);

            // --- Calculate omega from tA and sA
            //     (cheaper than using zA with preconditioned tA)
            omega = gSumProd(tA, sA)/tAtA;

            // --- Update solution and residual
            for (label cell=0; cell<nCells; cell++)
            {
                xPtr[cell] += alpha*wPtr[cell] + omega*zAPtr[cell];
                rAPtr[cell] = sAPtr[cell] - omega*tAPtr[cell];
            }

            solverPerf.finalResidual() = gSumMag(rA)/normFactor;
        } while
        (
            (++solverPerf.nIterations() < maxIter_
	     && 
             !(solverPerf.checkConvergence
               (
                   tolerance_, 
                   relTol_, 
                   solverPerf.nIterations(), 
                   minIter_
               )
              )
             )
        );
