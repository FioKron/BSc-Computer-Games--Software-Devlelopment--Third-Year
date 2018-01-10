#include "PlayerHoverTank.h"

// Structures:

struct MODEL_CONSTANT_BUFFER
{
	XMMATRIX WorldViewProjectionMatrix; // '64 bytes ( 4 x 4 = 16 floats x 4 bytes)'
	XMVECTOR DirectionalLightVector; // '16 bytes'
	XMVECTOR DirectionalLightColour; // '16 bytes'
	XMVECTOR AmbientLightColour; // '16 bytes'
}; // 'TOTAL SIZE = 112 bytes'

// Initialise (for default values, also use an initialiser list for such):
PlayerHoverTank::PlayerHoverTank(ID3D11Device*& NewD3DDeviceReference, ID3D11DeviceContext*& 
	NewD3DDeviceContextReference, HRESULT& ResultHandleReference, XMFLOAT3& InitialPosition) 
	: ControlledObject(NewD3DDeviceReference, NewD3DDeviceContextReference, ResultHandleReference,
		InitialPosition)
{
	PlayerCamera = new Camera();
	PlayerResultHandleReference = ResultHandleReference;
	EnergyCapsulesCollected = 0;
	CurrentHealth = DEFAULT_INITIAL_HEALTH;
	PlayerIsAlive = true;
	PlayerHasWon = false;
}

// Clean-up:
PlayerHoverTank::~PlayerHoverTank()
{
	if (PlayerCamera) delete PlayerCamera; 
	PlayerCamera = nullptr;
}

Camera*& PlayerHoverTank::GetPlayerCameraReference()
{
	return PlayerCamera;
}

bool PlayerHoverTank::GetPlayerHasWon()
{
	return PlayerHasWon;
}

bool PlayerHoverTank::GetPlayerIsAlive()
{
	return PlayerIsAlive;
}

// Handle the camera's offset as well:
void PlayerHoverTank::Draw(XMMATRIX* View, XMMATRIX* Projection)
{
	// Also taken from the tutorial mentioned in the header file 
	// (of the Camera class, as parts of this function was taken 
	// from Camera.cpp):

	// Perform degrees to radians conversions first though:
	float PitchRotationRadians = XMConvertToRadians(RotationVector.x);
	float YawRotationRadians = XMConvertToRadians(RotationVector.y);
	float RollRotationRadians = XMConvertToRadians(RotationVector.z);

	// Only handle the procedure for drawing the Player, if they
	// are still alive:
	if (PlayerIsAlive)
	{
		RotationMatrix = XMMatrixRotationRollPitchYaw(PitchRotationRadians,
			YawRotationRadians, RollRotationRadians);
		ObjectForwardTarget = XMVector3TransformCoord(DefaultForwardDirection,
			RotationMatrix);
		ObjectForwardTarget = XMVector3Normalize(ObjectForwardTarget);

		XMMATRIX RotateYTempMatrix = XMMatrixRotationY(YawRotationRadians);

		ObjectRightDirection = XMVector3TransformCoord(DefaultRightDirection, RotateYTempMatrix);
		ObjectUpDirection = XMVector3TransformCoord(DefaultUpDirection, RotateYTempMatrix);
		ObjectForwardDirection = XMVector3TransformCoord(DefaultForwardDirection, RotateYTempMatrix);

		// Update the position (and current movement direction), for movement:
		XMVECTOR HorizontalMovementVelocity = MovementLeftRight * ObjectRightDirection;
		XMVECTOR VerticalMovementVelocity = MovementUpDown * ObjectUpDirection;
		XMVECTOR DepthMovementVelocity = MovementForwardBackwards * ObjectForwardDirection;

		PositionVector += HorizontalMovementVelocity;
		PositionVector += DepthMovementVelocity;

		// Set the movement-direction to HorizontalMovementVelocity
		// before incrementing it by depth and height:
		ObjectMovementDirection = HorizontalMovementVelocity;
		ObjectMovementDirection += DepthMovementVelocity;
		ObjectMovementDirection += VerticalMovementVelocity;
		ObjectMovementDirection = XMVector3Normalize(ObjectMovementDirection);

		// Convert this vector to FLOAT3, to store in PositionFloat3:
		XMStoreFloat3(&PositionFloat3, PositionVector);

		// Then set the new camera target with this:
		PlayerCamera->SetTarget(PositionFloat3);

		MovementLeftRight = 0.0f;
		MovementForwardBackwards = 0.0f;
		MovementUpDown = 0.0f;

		ObjectForwardTarget += PositionVector;

		// Update the World transform matrix and the buffers now:

		// Scale first, then rotation, with translation as the last of 
		// the transformation actions to take place:
		World = XMMatrixScalingFromVector(ObjectScaleVector);
		World *= XMMatrixRotationRollPitchYaw(PitchRotationRadians,
			YawRotationRadians, RollRotationRadians);
		World *= XMMatrixTranslationFromVector(PositionVector);

		// For lighting on this object:
		XMMATRIX TransposeMatrix = XMMATRIX();
		MODEL_CONSTANT_BUFFER ObjectConstantBufferValues;
		ObjectConstantBufferValues.WorldViewProjectionMatrix = World * (*View) * (*Projection);

		ObjectConstantBufferValues.DirectionalLightColour = DirectionalLightColour;
		ObjectConstantBufferValues.AmbientLightColour = AmbientLightColour;
		ObjectConstantBufferValues.DirectionalLightVector = XMVector3Transform(DirectionalLightShinesFrom,
			TransposeMatrix);
		ObjectConstantBufferValues.DirectionalLightVector = XMVector3Normalize(ObjectConstantBufferValues.
			DirectionalLightVector);

		ImmediateContextReference->VSSetConstantBuffers(0u, 1u, &GameObjectConstantBufferReference);
		ImmediateContextReference->UpdateSubresource(GameObjectConstantBufferReference, 0u, nullptr,
			&ObjectConstantBufferValues, 0u, 0u);

		// Set this model's shaders and input layout as active:
		ImmediateContextReference->VSSetShader(GameObjectVertexShaderReference, nullptr, 0);
		ImmediateContextReference->PSSetShader(GameObjectPixelShaderReference, nullptr, 0);
		ImmediateContextReference->IASetInputLayout(GameObjectInputLayoutReference);

		ObjectModelReference->Draw();
	}
}

// Heal or damage the Player:
void PlayerHoverTank::ModifyHealth(int HealthModificationValue)
{
	CurrentHealth -= HealthModificationValue;

	// Then check if they are still alive:
	if (CurrentHealth <= 0)
	{
		PlayerIsAlive = false;
	}
}

// Increment their EnergyCapsulesCollected member:
void PlayerHoverTank::CapsuleCollected()
{
	EnergyCapsulesCollected++;

	// Then check if they have won:
	if (EnergyCapsulesCollected == GOAL_CAPSULE_COUNT)
	{
		PlayerHasWon = true;
	}
}
