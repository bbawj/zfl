��	
��
D
AddV2
x"T
y"T
z"T"
Ttype:
2	��
E
AssignAddVariableOp
resource
value"dtype"
dtypetype�
^
AssignVariableOp
resource
value"dtype"
dtypetype"
validate_shapebool( �
�
BiasAdd

value"T	
bias"T
output"T""
Ttype:
2	"-
data_formatstringNHWC:
NHWCNCHW
�
BiasAddGrad
out_backprop"T
output"T""
Ttype:
2	"-
data_formatstringNHWC:
NHWCNCHW
R
BroadcastGradientArgs
s0"T
s1"T
r0"T
r1"T"
Ttype0:
2	
N
Cast	
x"SrcT	
y"DstT"
SrcTtype"
DstTtype"
Truncatebool( 
h
ConcatV2
values"T*N
axis"Tidx
output"T"
Nint(0"	
Ttype"
Tidxtype0:
2	
8
Const
output"dtype"
valuetensor"
dtypetype
$
DisableCopyOnRead
resource�
9
DivNoNan
x"T
y"T
z"T"
Ttype:

2
W

ExpandDims

input"T
dim"Tdim
output"T"	
Ttype"
Tdimtype0:
2	
.
Identity

input"T
output"T"	
Ttype
9
	IdentityN

input2T
output2T"
T
list(type)(0
?

LogSoftmax
logits"T

logsoftmax"T"
Ttype:
2
u
MatMul
a"T
b"T
product"T"
transpose_abool( "
transpose_bbool( "
Ttype:
2	
�
MergeV2Checkpoints
checkpoint_prefixes
destination_prefix"
delete_old_dirsbool("
allow_missing_filesbool( �
?
Mul
x"T
y"T
z"T"
Ttype:
2	�
0
Neg
x"T
y"T"
Ttype:
2
	

NoOp
M
Pack
values"T*N
output"T"
Nint(0"	
Ttype"
axisint 
C
Placeholder
output"dtype"
dtypetype"
shapeshape:
f
Range
start"Tidx
limit"Tidx
delta"Tidx
output"Tidx" 
Tidxtype0:
2
	
@
ReadVariableOp
resource
value"dtype"
dtypetype�
E
Relu
features"T
activations"T"
Ttype:
2	
V
ReluGrad
	gradients"T
features"T
	backprops"T"
Ttype:
2	
[
Reshape
tensor"T
shape"Tshape
output"T"	
Ttype"
Tshapetype0:
2	
n
Restore
file_pattern
tensor_name
tensor"dt"

dttype"#
preferred_shardint����������
o
	RestoreV2

prefix
tensor_names
shape_and_slices
tensors2dtypes"
dtypes
list(type)(0�
I
Save
filename
tensor_names	
data2T"
T
list(type)(0�
l
SaveV2

prefix
tensor_names
shape_and_slices
tensors2dtypes"
dtypes
list(type)(0�
?
Select
	condition

t"T
e"T
output"T"	
Ttype
d
Shape

input"T&
output"out_type��out_type"	
Ttype"
out_typetype0:
2	
H
ShardedFilename
basename	
shard

num_shards
filename
O
Size

input"T
output"out_type"	
Ttype"
out_typetype0:
2	
a
Slice

input"T
begin"Index
size"Index
output"T"	
Ttype"
Indextype:
2	
9
Softmax
logits"T
softmax"T"
Ttype:
2
j
SoftmaxCrossEntropyWithLogits
features"T
labels"T	
loss"T
backprop"T"
Ttype:
2
�
StatefulPartitionedCall
args2Tin
output2Tout"
Tin
list(type)("
Tout
list(type)("	
ffunc"
configstring "
config_protostring "
executor_typestring ��
@
StaticRegexFullMatch	
input

output
"
patternstring
L

StringJoin
inputs*N

output"

Nint("
	separatorstring 
<
Sub
x"T
y"T
z"T"
Ttype:
2	
�
Sum

input"T
reduction_indices"Tidx
output"T"
	keep_dimsbool( ""
Ttype:
2	"
Tidxtype0:
2	
c
Tile

input"T
	multiples"
Tmultiples
output"T"	
Ttype"

Tmultiplestype0:
2	
�
VarHandleOp
resource"
	containerstring "
shared_namestring "
dtypetype"
shapeshape"#
allowed_deviceslist(string)
 �"serve*2.13.02v2.13.0-rc2-7-g1cb1a030a628��
|
SGD/m/dense_2/biasVarHandleOp*
_output_shapes
: *
dtype0*
shape:
*#
shared_nameSGD/m/dense_2/bias
u
&SGD/m/dense_2/bias/Read/ReadVariableOpReadVariableOpSGD/m/dense_2/bias*
_output_shapes
:
*
dtype0
�
SGD/m/dense_2/kernelVarHandleOp*
_output_shapes
: *
dtype0*
shape:	�
*%
shared_nameSGD/m/dense_2/kernel
~
(SGD/m/dense_2/kernel/Read/ReadVariableOpReadVariableOpSGD/m/dense_2/kernel*
_output_shapes
:	�
*
dtype0
}
SGD/m/dense_1/biasVarHandleOp*
_output_shapes
: *
dtype0*
shape:�*#
shared_nameSGD/m/dense_1/bias
v
&SGD/m/dense_1/bias/Read/ReadVariableOpReadVariableOpSGD/m/dense_1/bias*
_output_shapes	
:�*
dtype0
�
SGD/m/dense_1/kernelVarHandleOp*
_output_shapes
: *
dtype0*
shape:
��*%
shared_nameSGD/m/dense_1/kernel

(SGD/m/dense_1/kernel/Read/ReadVariableOpReadVariableOpSGD/m/dense_1/kernel* 
_output_shapes
:
��*
dtype0
n
learning_rateVarHandleOp*
_output_shapes
: *
dtype0*
shape: *
shared_namelearning_rate
g
!learning_rate/Read/ReadVariableOpReadVariableOplearning_rate*
_output_shapes
: *
dtype0
f
	iterationVarHandleOp*
_output_shapes
: *
dtype0	*
shape: *
shared_name	iteration
_
iteration/Read/ReadVariableOpReadVariableOp	iteration*
_output_shapes
: *
dtype0	
p
dense_2/biasVarHandleOp*
_output_shapes
: *
dtype0*
shape:
*
shared_namedense_2/bias
i
 dense_2/bias/Read/ReadVariableOpReadVariableOpdense_2/bias*
_output_shapes
:
*
dtype0
y
dense_2/kernelVarHandleOp*
_output_shapes
: *
dtype0*
shape:	�
*
shared_namedense_2/kernel
r
"dense_2/kernel/Read/ReadVariableOpReadVariableOpdense_2/kernel*
_output_shapes
:	�
*
dtype0
q
dense_1/biasVarHandleOp*
_output_shapes
: *
dtype0*
shape:�*
shared_namedense_1/bias
j
 dense_1/bias/Read/ReadVariableOpReadVariableOpdense_1/bias*
_output_shapes	
:�*
dtype0
z
dense_1/kernelVarHandleOp*
_output_shapes
: *
dtype0*
shape:
��*
shared_namedense_1/kernel
s
"dense_1/kernel/Read/ReadVariableOpReadVariableOpdense_1/kernel* 
_output_shapes
:
��*
dtype0
r
infer_xPlaceholder*+
_output_shapes
:���������*
dtype0* 
shape:���������
�
StatefulPartitionedCallStatefulPartitionedCallinfer_xdense_1/kerneldense_1/biasdense_2/kerneldense_2/bias*
Tin	
2*
Tout
2*
_collective_manager_ids
 *:
_output_shapes(
&:���������
:���������
*&
_read_only_resource_inputs
*0
config_proto 

CPU

GPU2*0J 8� *,
f'R%
#__inference_signature_wrapper_90649
X
restore_checkpoint_pathPlaceholder*
_output_shapes
: *
dtype0*
shape: 
�
StatefulPartitionedCall_1StatefulPartitionedCallrestore_checkpoint_pathdense_1/kerneldense_1/biasdense_2/kerneldense_2/bias*
Tin	
2*
Tout
2*
_collective_manager_ids
 *$
_output_shapes
::::* 
_read_only_resource_inputs
 *0
config_proto 

CPU

GPU2*0J 8� *,
f'R%
#__inference_signature_wrapper_90683
U
save_checkpoint_pathPlaceholder*
_output_shapes
: *
dtype0*
shape: 
�
StatefulPartitionedCall_2StatefulPartitionedCallsave_checkpoint_pathdense_1/kerneldense_1/biasdense_2/kerneldense_2/bias*
Tin	
2*
Tout
2*
_collective_manager_ids
 *
_output_shapes
: *&
_read_only_resource_inputs
*0
config_proto 

CPU

GPU2*0J 8� *,
f'R%
#__inference_signature_wrapper_90663
r
train_xPlaceholder*+
_output_shapes
:���������*
dtype0* 
shape:���������
j
train_yPlaceholder*'
_output_shapes
:���������
*
dtype0*
shape:���������

�
StatefulPartitionedCall_3StatefulPartitionedCalltrain_xtrain_ydense_1/kerneldense_1/biasdense_2/kerneldense_2/biaslearning_rateSGD/m/dense_1/kernelSGD/m/dense_1/biasSGD/m/dense_2/kernelSGD/m/dense_2/bias	iteration*
Tin
2*
Tout
2*
_collective_manager_ids
 *
_output_shapes
: *#
_read_only_resource_inputs
*0
config_proto 

CPU

GPU2*0J 8� *,
f'R%
#__inference_signature_wrapper_90633

NoOpNoOp
�
ConstConst"/device:CPU:0*
_output_shapes
: *
dtype0*�
value�B� B�
L
	model
	infer
restore
save
	train

signatures*
�
layer-0
layer_with_weights-0
layer-1
	layer_with_weights-1
	layer-2

	variables
trainable_variables
regularization_losses
	keras_api
__call__
*&call_and_return_all_conditional_losses
_default_save_signature
	optimizer*

trace_0* 

trace_0* 

trace_0* 

trace_0* 
/
	train
	infer
save
restore* 
�
	variables
trainable_variables
regularization_losses
	keras_api
__call__
*&call_and_return_all_conditional_losses* 
�
 	variables
!trainable_variables
"regularization_losses
#	keras_api
$__call__
*%&call_and_return_all_conditional_losses

&kernel
'bias*
�
(	variables
)trainable_variables
*regularization_losses
+	keras_api
,__call__
*-&call_and_return_all_conditional_losses

.kernel
/bias*
 
&0
'1
.2
/3*
 
&0
'1
.2
/3*
* 
�
0non_trainable_variables

1layers
2metrics
3layer_regularization_losses
4layer_metrics

	variables
trainable_variables
regularization_losses
__call__
_default_save_signature
*&call_and_return_all_conditional_losses
&"call_and_return_conditional_losses*

5trace_0
6trace_1* 

7trace_0
8trace_1* 
* 
o
9
_variables
:_iterations
;_learning_rate
<_index_dict
=	momentums
>_update_step_xla*
* 
* 
* 
* 
* 
* 
* 
* 
* 
* 
* 
�
?non_trainable_variables

@layers
Ametrics
Blayer_regularization_losses
Clayer_metrics
	variables
trainable_variables
regularization_losses
__call__
*&call_and_return_all_conditional_losses
&"call_and_return_conditional_losses* 

Dtrace_0* 

Etrace_0* 

&0
'1*

&0
'1*
* 
�
Fnon_trainable_variables

Glayers
Hmetrics
Ilayer_regularization_losses
Jlayer_metrics
 	variables
!trainable_variables
"regularization_losses
$__call__
*%&call_and_return_all_conditional_losses
&%"call_and_return_conditional_losses*

Ktrace_0* 

Ltrace_0* 
d^
VARIABLE_VALUEdense_1/kernel<model/layer_with_weights-0/kernel/.ATTRIBUTES/VARIABLE_VALUE*
`Z
VARIABLE_VALUEdense_1/bias:model/layer_with_weights-0/bias/.ATTRIBUTES/VARIABLE_VALUE*

.0
/1*

.0
/1*
* 
�
Mnon_trainable_variables

Nlayers
Ometrics
Player_regularization_losses
Qlayer_metrics
(	variables
)trainable_variables
*regularization_losses
,__call__
*-&call_and_return_all_conditional_losses
&-"call_and_return_conditional_losses*

Rtrace_0* 

Strace_0* 
d^
VARIABLE_VALUEdense_2/kernel<model/layer_with_weights-1/kernel/.ATTRIBUTES/VARIABLE_VALUE*
`Z
VARIABLE_VALUEdense_2/bias:model/layer_with_weights-1/bias/.ATTRIBUTES/VARIABLE_VALUE*
* 

0
1
	2*
* 
* 
* 
* 
* 
* 
* 
'
:0
T1
U2
V3
W4*
YS
VARIABLE_VALUE	iteration6model/optimizer/_iterations/.ATTRIBUTES/VARIABLE_VALUE*
`Z
VARIABLE_VALUElearning_rate9model/optimizer/_learning_rate/.ATTRIBUTES/VARIABLE_VALUE*
* 
 
T0
U1
V2
W3*
6
Xtrace_0
Ytrace_1
Ztrace_2
[trace_3* 
* 
* 
* 
* 
* 
* 
* 
* 
* 
* 
* 
* 
* 
* 
* 
* 
* 
* 
* 
* 
* 
e_
VARIABLE_VALUESGD/m/dense_1/kernel7model/optimizer/_variables/1/.ATTRIBUTES/VARIABLE_VALUE*
c]
VARIABLE_VALUESGD/m/dense_1/bias7model/optimizer/_variables/2/.ATTRIBUTES/VARIABLE_VALUE*
e_
VARIABLE_VALUESGD/m/dense_2/kernel7model/optimizer/_variables/3/.ATTRIBUTES/VARIABLE_VALUE*
c]
VARIABLE_VALUESGD/m/dense_2/bias7model/optimizer/_variables/4/.ATTRIBUTES/VARIABLE_VALUE*
* 
* 
* 
* 
O
saver_filenamePlaceholder*
_output_shapes
: *
dtype0*
shape: 
�
StatefulPartitionedCall_4StatefulPartitionedCallsaver_filenamedense_1/kerneldense_1/biasdense_2/kerneldense_2/bias	iterationlearning_rateSGD/m/dense_1/kernelSGD/m/dense_1/biasSGD/m/dense_2/kernelSGD/m/dense_2/biasConst*
Tin
2*
Tout
2*
_collective_manager_ids
 *
_output_shapes
: * 
_read_only_resource_inputs
 *0
config_proto 

CPU

GPU2*0J 8� *'
f"R 
__inference__traced_save_91279
�
StatefulPartitionedCall_5StatefulPartitionedCallsaver_filenamedense_1/kerneldense_1/biasdense_2/kerneldense_2/bias	iterationlearning_rateSGD/m/dense_1/kernelSGD/m/dense_1/biasSGD/m/dense_2/kernelSGD/m/dense_2/bias*
Tin
2*
Tout
2*
_collective_manager_ids
 *
_output_shapes
: * 
_read_only_resource_inputs
 *0
config_proto 

CPU

GPU2*0J 8� **
f%R#
!__inference__traced_restore_91318��
�

�
B__inference_dense_1_layer_call_and_return_conditional_losses_91113

inputs2
matmul_readvariableop_resource:
��.
biasadd_readvariableop_resource:	�
identity��BiasAdd/ReadVariableOp�MatMul/ReadVariableOpv
MatMul/ReadVariableOpReadVariableOpmatmul_readvariableop_resource* 
_output_shapes
:
��*
dtype0j
MatMulMatMulinputsMatMul/ReadVariableOp:value:0*
T0*(
_output_shapes
:����������s
BiasAdd/ReadVariableOpReadVariableOpbiasadd_readvariableop_resource*
_output_shapes	
:�*
dtype0w
BiasAddBiasAddMatMul:product:0BiasAdd/ReadVariableOp:value:0*
T0*(
_output_shapes
:����������Q
ReluReluBiasAdd:output:0*
T0*(
_output_shapes
:����������b
IdentityIdentityRelu:activations:0^NoOp*
T0*(
_output_shapes
:����������S
NoOpNoOp^BiasAdd/ReadVariableOp^MatMul/ReadVariableOp*
_output_shapes
 "
identityIdentity:output:0*(
_construction_contextkEagerRuntime*+
_input_shapes
:����������: : 20
BiasAdd/ReadVariableOpBiasAdd/ReadVariableOp2.
MatMul/ReadVariableOpMatMul/ReadVariableOp:($
"
_user_specified_name
resource:($
"
_user_specified_name
resource:P L
(
_output_shapes
:����������
 
_user_specified_nameinputs
��
�
__inference_train_90935
x
yE
1sequential_dense_1_matmul_readvariableop_resource:
��A
2sequential_dense_1_biasadd_readvariableop_resource:	�D
1sequential_dense_2_matmul_readvariableop_resource:	�
@
2sequential_dense_2_biasadd_readvariableop_resource:

	sgd_90874: 
	sgd_90876:
��
	sgd_90894:	�
	sgd_90912:	�

	sgd_90930:
*
 sgd_assignaddvariableop_resource:	 

identity_4��SGD/AssignAddVariableOp�SGD/StatefulPartitionedCall�SGD/StatefulPartitionedCall_1�SGD/StatefulPartitionedCall_2�SGD/StatefulPartitionedCall_3�)sequential/dense_1/BiasAdd/ReadVariableOp�(sequential/dense_1/MatMul/ReadVariableOp�)sequential/dense_2/BiasAdd/ReadVariableOp�(sequential/dense_2/MatMul/ReadVariableOpi
sequential/flatten/ConstConst*
_output_shapes
:*
dtype0*
valueB"����  ~
sequential/flatten/ReshapeReshapex!sequential/flatten/Const:output:0*
T0*(
_output_shapes
:�����������
(sequential/dense_1/MatMul/ReadVariableOpReadVariableOp1sequential_dense_1_matmul_readvariableop_resource* 
_output_shapes
:
��*
dtype0�
sequential/dense_1/MatMulMatMul#sequential/flatten/Reshape:output:00sequential/dense_1/MatMul/ReadVariableOp:value:0*
T0*(
_output_shapes
:�����������
)sequential/dense_1/BiasAdd/ReadVariableOpReadVariableOp2sequential_dense_1_biasadd_readvariableop_resource*
_output_shapes	
:�*
dtype0�
sequential/dense_1/BiasAddBiasAdd#sequential/dense_1/MatMul:product:01sequential/dense_1/BiasAdd/ReadVariableOp:value:0*
T0*(
_output_shapes
:����������w
sequential/dense_1/ReluRelu#sequential/dense_1/BiasAdd:output:0*
T0*(
_output_shapes
:�����������
(sequential/dense_2/MatMul/ReadVariableOpReadVariableOp1sequential_dense_2_matmul_readvariableop_resource*
_output_shapes
:	�
*
dtype0�
sequential/dense_2/MatMulMatMul%sequential/dense_1/Relu:activations:00sequential/dense_2/MatMul/ReadVariableOp:value:0*
T0*'
_output_shapes
:���������
�
)sequential/dense_2/BiasAdd/ReadVariableOpReadVariableOp2sequential_dense_2_biasadd_readvariableop_resource*
_output_shapes
:
*
dtype0�
sequential/dense_2/BiasAddBiasAdd#sequential/dense_2/MatMul:product:01sequential/dense_2/BiasAdd/ReadVariableOp:value:0*
T0*'
_output_shapes
:���������
c
categorical_crossentropy/ConstConst*
_output_shapes
: *
dtype0*
valueB
 *    �
?categorical_crossentropy/softmax_cross_entropy_with_logits/RankConst*
_output_shapes
: *
dtype0*
value	B :�
@categorical_crossentropy/softmax_cross_entropy_with_logits/ShapeShape#sequential/dense_2/BiasAdd:output:0*
T0*
_output_shapes
::���
Acategorical_crossentropy/softmax_cross_entropy_with_logits/Rank_1Const*
_output_shapes
: *
dtype0*
value	B :�
Bcategorical_crossentropy/softmax_cross_entropy_with_logits/Shape_1Shape#sequential/dense_2/BiasAdd:output:0*
T0*
_output_shapes
::���
@categorical_crossentropy/softmax_cross_entropy_with_logits/Sub/yConst*
_output_shapes
: *
dtype0*
value	B :�
>categorical_crossentropy/softmax_cross_entropy_with_logits/SubSubJcategorical_crossentropy/softmax_cross_entropy_with_logits/Rank_1:output:0Icategorical_crossentropy/softmax_cross_entropy_with_logits/Sub/y:output:0*
T0*
_output_shapes
: �
Fcategorical_crossentropy/softmax_cross_entropy_with_logits/Slice/beginPackBcategorical_crossentropy/softmax_cross_entropy_with_logits/Sub:z:0*
N*
T0*
_output_shapes
:�
Ecategorical_crossentropy/softmax_cross_entropy_with_logits/Slice/sizeConst*
_output_shapes
:*
dtype0*
valueB:�
@categorical_crossentropy/softmax_cross_entropy_with_logits/SliceSliceKcategorical_crossentropy/softmax_cross_entropy_with_logits/Shape_1:output:0Ocategorical_crossentropy/softmax_cross_entropy_with_logits/Slice/begin:output:0Ncategorical_crossentropy/softmax_cross_entropy_with_logits/Slice/size:output:0*
Index0*
T0*
_output_shapes
:�
Jcategorical_crossentropy/softmax_cross_entropy_with_logits/concat/values_0Const*
_output_shapes
:*
dtype0*
valueB:
����������
Fcategorical_crossentropy/softmax_cross_entropy_with_logits/concat/axisConst*
_output_shapes
: *
dtype0*
value	B : �
Acategorical_crossentropy/softmax_cross_entropy_with_logits/concatConcatV2Scategorical_crossentropy/softmax_cross_entropy_with_logits/concat/values_0:output:0Icategorical_crossentropy/softmax_cross_entropy_with_logits/Slice:output:0Ocategorical_crossentropy/softmax_cross_entropy_with_logits/concat/axis:output:0*
N*
T0*
_output_shapes
:�
Bcategorical_crossentropy/softmax_cross_entropy_with_logits/ReshapeReshape#sequential/dense_2/BiasAdd:output:0Jcategorical_crossentropy/softmax_cross_entropy_with_logits/concat:output:0*
T0*0
_output_shapes
:�������������������
Acategorical_crossentropy/softmax_cross_entropy_with_logits/Rank_2Const*
_output_shapes
: *
dtype0*
value	B :�
Bcategorical_crossentropy/softmax_cross_entropy_with_logits/Shape_2Shapey*
T0*
_output_shapes
::���
Bcategorical_crossentropy/softmax_cross_entropy_with_logits/Sub_1/yConst*
_output_shapes
: *
dtype0*
value	B :�
@categorical_crossentropy/softmax_cross_entropy_with_logits/Sub_1SubJcategorical_crossentropy/softmax_cross_entropy_with_logits/Rank_2:output:0Kcategorical_crossentropy/softmax_cross_entropy_with_logits/Sub_1/y:output:0*
T0*
_output_shapes
: �
Hcategorical_crossentropy/softmax_cross_entropy_with_logits/Slice_1/beginPackDcategorical_crossentropy/softmax_cross_entropy_with_logits/Sub_1:z:0*
N*
T0*
_output_shapes
:�
Gcategorical_crossentropy/softmax_cross_entropy_with_logits/Slice_1/sizeConst*
_output_shapes
:*
dtype0*
valueB:�
Bcategorical_crossentropy/softmax_cross_entropy_with_logits/Slice_1SliceKcategorical_crossentropy/softmax_cross_entropy_with_logits/Shape_2:output:0Qcategorical_crossentropy/softmax_cross_entropy_with_logits/Slice_1/begin:output:0Pcategorical_crossentropy/softmax_cross_entropy_with_logits/Slice_1/size:output:0*
Index0*
T0*
_output_shapes
:�
Lcategorical_crossentropy/softmax_cross_entropy_with_logits/concat_1/values_0Const*
_output_shapes
:*
dtype0*
valueB:
����������
Hcategorical_crossentropy/softmax_cross_entropy_with_logits/concat_1/axisConst*
_output_shapes
: *
dtype0*
value	B : �
Ccategorical_crossentropy/softmax_cross_entropy_with_logits/concat_1ConcatV2Ucategorical_crossentropy/softmax_cross_entropy_with_logits/concat_1/values_0:output:0Kcategorical_crossentropy/softmax_cross_entropy_with_logits/Slice_1:output:0Qcategorical_crossentropy/softmax_cross_entropy_with_logits/concat_1/axis:output:0*
N*
T0*
_output_shapes
:�
Dcategorical_crossentropy/softmax_cross_entropy_with_logits/Reshape_1ReshapeyLcategorical_crossentropy/softmax_cross_entropy_with_logits/concat_1:output:0*
T0*0
_output_shapes
:�������������������
:categorical_crossentropy/softmax_cross_entropy_with_logitsSoftmaxCrossEntropyWithLogitsKcategorical_crossentropy/softmax_cross_entropy_with_logits/Reshape:output:0Mcategorical_crossentropy/softmax_cross_entropy_with_logits/Reshape_1:output:0*
T0*?
_output_shapes-
+:���������:�������������������
Bcategorical_crossentropy/softmax_cross_entropy_with_logits/Sub_2/yConst*
_output_shapes
: *
dtype0*
value	B :�
@categorical_crossentropy/softmax_cross_entropy_with_logits/Sub_2SubHcategorical_crossentropy/softmax_cross_entropy_with_logits/Rank:output:0Kcategorical_crossentropy/softmax_cross_entropy_with_logits/Sub_2/y:output:0*
T0*
_output_shapes
: �
Hcategorical_crossentropy/softmax_cross_entropy_with_logits/Slice_2/beginConst*
_output_shapes
:*
dtype0*
valueB: �
Gcategorical_crossentropy/softmax_cross_entropy_with_logits/Slice_2/sizePackDcategorical_crossentropy/softmax_cross_entropy_with_logits/Sub_2:z:0*
N*
T0*
_output_shapes
:�
Bcategorical_crossentropy/softmax_cross_entropy_with_logits/Slice_2SliceIcategorical_crossentropy/softmax_cross_entropy_with_logits/Shape:output:0Qcategorical_crossentropy/softmax_cross_entropy_with_logits/Slice_2/begin:output:0Pcategorical_crossentropy/softmax_cross_entropy_with_logits/Slice_2/size:output:0*
Index0*
T0*
_output_shapes
:�
Dcategorical_crossentropy/softmax_cross_entropy_with_logits/Reshape_2ReshapeAcategorical_crossentropy/softmax_cross_entropy_with_logits:loss:0Kcategorical_crossentropy/softmax_cross_entropy_with_logits/Slice_2:output:0*
T0*#
_output_shapes
:���������q
,categorical_crossentropy/weighted_loss/ConstConst*
_output_shapes
: *
dtype0*
valueB
 *  �?�
*categorical_crossentropy/weighted_loss/MulMulMcategorical_crossentropy/softmax_cross_entropy_with_logits/Reshape_2:output:05categorical_crossentropy/weighted_loss/Const:output:0*
T0*#
_output_shapes
:���������x
.categorical_crossentropy/weighted_loss/Const_1Const*
_output_shapes
:*
dtype0*
valueB: �
*categorical_crossentropy/weighted_loss/SumSum.categorical_crossentropy/weighted_loss/Mul:z:07categorical_crossentropy/weighted_loss/Const_1:output:0*
T0*
_output_shapes
: �
3categorical_crossentropy/weighted_loss/num_elementsSize.categorical_crossentropy/weighted_loss/Mul:z:0*
T0*
_output_shapes
: �
8categorical_crossentropy/weighted_loss/num_elements/CastCast<categorical_crossentropy/weighted_loss/num_elements:output:0*

DstT0*

SrcT0*
_output_shapes
: m
+categorical_crossentropy/weighted_loss/RankConst*
_output_shapes
: *
dtype0*
value	B : t
2categorical_crossentropy/weighted_loss/range/startConst*
_output_shapes
: *
dtype0*
value	B : t
2categorical_crossentropy/weighted_loss/range/deltaConst*
_output_shapes
: *
dtype0*
value	B :�
,categorical_crossentropy/weighted_loss/rangeRange;categorical_crossentropy/weighted_loss/range/start:output:04categorical_crossentropy/weighted_loss/Rank:output:0;categorical_crossentropy/weighted_loss/range/delta:output:0*
_output_shapes
: �
,categorical_crossentropy/weighted_loss/Sum_1Sum3categorical_crossentropy/weighted_loss/Sum:output:05categorical_crossentropy/weighted_loss/range:output:0*
T0*
_output_shapes
: �
,categorical_crossentropy/weighted_loss/valueDivNoNan5categorical_crossentropy/weighted_loss/Sum_1:output:0<categorical_crossentropy/weighted_loss/num_elements/Cast:y:0*
T0*
_output_shapes
: I
onesConst*
_output_shapes
: *
dtype0*
valueB
 *  �?�
@gradient_tape/categorical_crossentropy/weighted_loss/value/ShapeConst*
_output_shapes
: *
dtype0*
valueB �
Bgradient_tape/categorical_crossentropy/weighted_loss/value/Shape_1Const*
_output_shapes
: *
dtype0*
valueB �
Pgradient_tape/categorical_crossentropy/weighted_loss/value/BroadcastGradientArgsBroadcastGradientArgsIgradient_tape/categorical_crossentropy/weighted_loss/value/Shape:output:0Kgradient_tape/categorical_crossentropy/weighted_loss/value/Shape_1:output:0*2
_output_shapes 
:���������:����������
Egradient_tape/categorical_crossentropy/weighted_loss/value/div_no_nanDivNoNanones:output:0<categorical_crossentropy/weighted_loss/num_elements/Cast:y:0*
T0*
_output_shapes
: �
>gradient_tape/categorical_crossentropy/weighted_loss/value/SumSumIgradient_tape/categorical_crossentropy/weighted_loss/value/div_no_nan:z:0Ugradient_tape/categorical_crossentropy/weighted_loss/value/BroadcastGradientArgs:r0:0*
T0*
_output_shapes
: �
Bgradient_tape/categorical_crossentropy/weighted_loss/value/ReshapeReshapeGgradient_tape/categorical_crossentropy/weighted_loss/value/Sum:output:0Igradient_tape/categorical_crossentropy/weighted_loss/value/Shape:output:0*
T0*
_output_shapes
: �
>gradient_tape/categorical_crossentropy/weighted_loss/value/NegNeg5categorical_crossentropy/weighted_loss/Sum_1:output:0*
T0*
_output_shapes
: �
Ggradient_tape/categorical_crossentropy/weighted_loss/value/div_no_nan_1DivNoNanBgradient_tape/categorical_crossentropy/weighted_loss/value/Neg:y:0<categorical_crossentropy/weighted_loss/num_elements/Cast:y:0*
T0*
_output_shapes
: �
Ggradient_tape/categorical_crossentropy/weighted_loss/value/div_no_nan_2DivNoNanKgradient_tape/categorical_crossentropy/weighted_loss/value/div_no_nan_1:z:0<categorical_crossentropy/weighted_loss/num_elements/Cast:y:0*
T0*
_output_shapes
: �
>gradient_tape/categorical_crossentropy/weighted_loss/value/mulMulones:output:0Kgradient_tape/categorical_crossentropy/weighted_loss/value/div_no_nan_2:z:0*
T0*
_output_shapes
: �
@gradient_tape/categorical_crossentropy/weighted_loss/value/Sum_1SumBgradient_tape/categorical_crossentropy/weighted_loss/value/mul:z:0Ugradient_tape/categorical_crossentropy/weighted_loss/value/BroadcastGradientArgs:r1:0*
T0*
_output_shapes
: �
Dgradient_tape/categorical_crossentropy/weighted_loss/value/Reshape_1ReshapeIgradient_tape/categorical_crossentropy/weighted_loss/value/Sum_1:output:0Kgradient_tape/categorical_crossentropy/weighted_loss/value/Shape_1:output:0*
T0*
_output_shapes
: �
Bgradient_tape/categorical_crossentropy/weighted_loss/Reshape/shapeConst*
_output_shapes
: *
dtype0*
valueB �
Dgradient_tape/categorical_crossentropy/weighted_loss/Reshape/shape_1Const*
_output_shapes
: *
dtype0*
valueB �
<gradient_tape/categorical_crossentropy/weighted_loss/ReshapeReshapeKgradient_tape/categorical_crossentropy/weighted_loss/value/Reshape:output:0Mgradient_tape/categorical_crossentropy/weighted_loss/Reshape/shape_1:output:0*
T0*
_output_shapes
: }
:gradient_tape/categorical_crossentropy/weighted_loss/ConstConst*
_output_shapes
: *
dtype0*
valueB �
9gradient_tape/categorical_crossentropy/weighted_loss/TileTileEgradient_tape/categorical_crossentropy/weighted_loss/Reshape:output:0Cgradient_tape/categorical_crossentropy/weighted_loss/Const:output:0*
T0*
_output_shapes
: �
Dgradient_tape/categorical_crossentropy/weighted_loss/Reshape_1/shapeConst*
_output_shapes
:*
dtype0*
valueB:�
>gradient_tape/categorical_crossentropy/weighted_loss/Reshape_1ReshapeBgradient_tape/categorical_crossentropy/weighted_loss/Tile:output:0Mgradient_tape/categorical_crossentropy/weighted_loss/Reshape_1/shape:output:0*
T0*
_output_shapes
:�
:gradient_tape/categorical_crossentropy/weighted_loss/ShapeShape.categorical_crossentropy/weighted_loss/Mul:z:0*
T0*
_output_shapes
::���
;gradient_tape/categorical_crossentropy/weighted_loss/Tile_1TileGgradient_tape/categorical_crossentropy/weighted_loss/Reshape_1:output:0Cgradient_tape/categorical_crossentropy/weighted_loss/Shape:output:0*
T0*#
_output_shapes
:����������
8gradient_tape/categorical_crossentropy/weighted_loss/MulMulDgradient_tape/categorical_crossentropy/weighted_loss/Tile_1:output:05categorical_crossentropy/weighted_loss/Const:output:0*
T0*#
_output_shapes
:����������
Ngradient_tape/categorical_crossentropy/softmax_cross_entropy_with_logits/ShapeShapeAcategorical_crossentropy/softmax_cross_entropy_with_logits:loss:0*
T0*
_output_shapes
::���
Pgradient_tape/categorical_crossentropy/softmax_cross_entropy_with_logits/ReshapeReshape<gradient_tape/categorical_crossentropy/weighted_loss/Mul:z:0Wgradient_tape/categorical_crossentropy/softmax_cross_entropy_with_logits/Shape:output:0*
T0*#
_output_shapes
:����������
Wgradient_tape/categorical_crossentropy/softmax_cross_entropy_with_logits/ExpandDims/dimConst*
_output_shapes
: *
dtype0*
valueB :
����������
Sgradient_tape/categorical_crossentropy/softmax_cross_entropy_with_logits/ExpandDims
ExpandDimsYgradient_tape/categorical_crossentropy/softmax_cross_entropy_with_logits/Reshape:output:0`gradient_tape/categorical_crossentropy/softmax_cross_entropy_with_logits/ExpandDims/dim:output:0*
T0*'
_output_shapes
:����������
Lgradient_tape/categorical_crossentropy/softmax_cross_entropy_with_logits/mulMul\gradient_tape/categorical_crossentropy/softmax_cross_entropy_with_logits/ExpandDims:output:0Ecategorical_crossentropy/softmax_cross_entropy_with_logits:backprop:0*
T0*0
_output_shapes
:�������������������
Sgradient_tape/categorical_crossentropy/softmax_cross_entropy_with_logits/LogSoftmax
LogSoftmaxKcategorical_crossentropy/softmax_cross_entropy_with_logits/Reshape:output:0*
T0*0
_output_shapes
:�������������������
Lgradient_tape/categorical_crossentropy/softmax_cross_entropy_with_logits/NegNeg`gradient_tape/categorical_crossentropy/softmax_cross_entropy_with_logits/LogSoftmax:logsoftmax:0*
T0*0
_output_shapes
:�������������������
Ygradient_tape/categorical_crossentropy/softmax_cross_entropy_with_logits/ExpandDims_1/dimConst*
_output_shapes
: *
dtype0*
valueB :
����������
Ugradient_tape/categorical_crossentropy/softmax_cross_entropy_with_logits/ExpandDims_1
ExpandDimsYgradient_tape/categorical_crossentropy/softmax_cross_entropy_with_logits/Reshape:output:0bgradient_tape/categorical_crossentropy/softmax_cross_entropy_with_logits/ExpandDims_1/dim:output:0*
T0*'
_output_shapes
:����������
Ngradient_tape/categorical_crossentropy/softmax_cross_entropy_with_logits/mul_1Mul^gradient_tape/categorical_crossentropy/softmax_cross_entropy_with_logits/ExpandDims_1:output:0Pgradient_tape/categorical_crossentropy/softmax_cross_entropy_with_logits/Neg:y:0*
T0*0
_output_shapes
:�������������������
Pgradient_tape/categorical_crossentropy/softmax_cross_entropy_with_logits/Shape_1Shape#sequential/dense_2/BiasAdd:output:0*
T0*
_output_shapes
::���
Rgradient_tape/categorical_crossentropy/softmax_cross_entropy_with_logits/Reshape_1ReshapePgradient_tape/categorical_crossentropy/softmax_cross_entropy_with_logits/mul:z:0Ygradient_tape/categorical_crossentropy/softmax_cross_entropy_with_logits/Shape_1:output:0*
T0*'
_output_shapes
:���������
�
4gradient_tape/sequential/dense_2/BiasAdd/BiasAddGradBiasAddGrad[gradient_tape/categorical_crossentropy/softmax_cross_entropy_with_logits/Reshape_1:output:0*
T0*
_output_shapes
:
�
.gradient_tape/sequential/dense_2/MatMul/MatMulMatMul[gradient_tape/categorical_crossentropy/softmax_cross_entropy_with_logits/Reshape_1:output:00sequential/dense_2/MatMul/ReadVariableOp:value:0*
T0*(
_output_shapes
:����������*
transpose_b(�
0gradient_tape/sequential/dense_2/MatMul/MatMul_1MatMul%sequential/dense_1/Relu:activations:0[gradient_tape/categorical_crossentropy/softmax_cross_entropy_with_logits/Reshape_1:output:0*
T0*
_output_shapes
:	�
*
transpose_a(�
)gradient_tape/sequential/dense_1/ReluGradReluGrad8gradient_tape/sequential/dense_2/MatMul/MatMul:product:0%sequential/dense_1/Relu:activations:0*
T0*(
_output_shapes
:�����������
4gradient_tape/sequential/dense_1/BiasAdd/BiasAddGradBiasAddGrad5gradient_tape/sequential/dense_1/ReluGrad:backprops:0*
T0*
_output_shapes	
:��
.gradient_tape/sequential/dense_1/MatMul/MatMulMatMul#sequential/flatten/Reshape:output:05gradient_tape/sequential/dense_1/ReluGrad:backprops:0*
T0* 
_output_shapes
:
��*
transpose_a(y
IdentityIdentity8gradient_tape/sequential/dense_1/MatMul/MatMul:product:0*
T0* 
_output_shapes
:
��{

Identity_1Identity=gradient_tape/sequential/dense_1/BiasAdd/BiasAddGrad:output:0*
T0*
_output_shapes	
:�|

Identity_2Identity:gradient_tape/sequential/dense_2/MatMul/MatMul_1:product:0*
T0*
_output_shapes
:	�
z

Identity_3Identity=gradient_tape/sequential/dense_2/BiasAdd/BiasAddGrad:output:0*
T0*
_output_shapes
:
�
	IdentityN	IdentityN8gradient_tape/sequential/dense_1/MatMul/MatMul:product:0=gradient_tape/sequential/dense_1/BiasAdd/BiasAddGrad:output:0:gradient_tape/sequential/dense_2/MatMul/MatMul_1:product:0=gradient_tape/sequential/dense_2/BiasAdd/BiasAddGrad:output:08gradient_tape/sequential/dense_1/MatMul/MatMul:product:0=gradient_tape/sequential/dense_1/BiasAdd/BiasAddGrad:output:0:gradient_tape/sequential/dense_2/MatMul/MatMul_1:product:0=gradient_tape/sequential/dense_2/BiasAdd/BiasAddGrad:output:0*
T

2*+
_gradient_op_typeCustomGradient-90846*\
_output_shapesJ
H:
��:�:	�
:
:
��:�:	�
:
�
SGD/StatefulPartitionedCallStatefulPartitionedCallIdentityN:output:01sequential_dense_1_matmul_readvariableop_resource	sgd_90874	sgd_90876)^sequential/dense_1/MatMul/ReadVariableOp*
Tin
2*

Tout
 *
_XlaMustCompile(*
_collective_manager_ids
 *
_output_shapes
 *#
_read_only_resource_inputs
*0
config_proto 

CPU

GPU2*0J 8� *+
f&R$
"__inference__update_step_xla_90872�
SGD/StatefulPartitionedCall_1StatefulPartitionedCallIdentityN:output:12sequential_dense_1_biasadd_readvariableop_resource	sgd_90874	sgd_90894*^sequential/dense_1/BiasAdd/ReadVariableOp*
Tin
2*

Tout
 *
_XlaMustCompile(*
_collective_manager_ids
 *
_output_shapes
 *#
_read_only_resource_inputs
*0
config_proto 

CPU

GPU2*0J 8� *+
f&R$
"__inference__update_step_xla_90891�
SGD/StatefulPartitionedCall_2StatefulPartitionedCallIdentityN:output:21sequential_dense_2_matmul_readvariableop_resource	sgd_90874	sgd_90912)^sequential/dense_2/MatMul/ReadVariableOp*
Tin
2*

Tout
 *
_XlaMustCompile(*
_collective_manager_ids
 *
_output_shapes
 *#
_read_only_resource_inputs
*0
config_proto 

CPU

GPU2*0J 8� *+
f&R$
"__inference__update_step_xla_90909�
SGD/StatefulPartitionedCall_3StatefulPartitionedCallIdentityN:output:32sequential_dense_2_biasadd_readvariableop_resource	sgd_90874	sgd_90930*^sequential/dense_2/BiasAdd/ReadVariableOp*
Tin
2*

Tout
 *
_XlaMustCompile(*
_collective_manager_ids
 *
_output_shapes
 *#
_read_only_resource_inputs
*0
config_proto 

CPU

GPU2*0J 8� *+
f&R$
"__inference__update_step_xla_90927K
	SGD/ConstConst*
_output_shapes
: *
dtype0	*
value	B	 R�
SGD/AssignAddVariableOpAssignAddVariableOp sgd_assignaddvariableop_resourceSGD/Const:output:0*
_output_shapes
 *
dtype0	p

Identity_4Identity0categorical_crossentropy/weighted_loss/value:z:0^NoOp*
T0*
_output_shapes
: �
NoOpNoOp^SGD/AssignAddVariableOp^SGD/StatefulPartitionedCall^SGD/StatefulPartitionedCall_1^SGD/StatefulPartitionedCall_2^SGD/StatefulPartitionedCall_3*^sequential/dense_1/BiasAdd/ReadVariableOp)^sequential/dense_1/MatMul/ReadVariableOp*^sequential/dense_2/BiasAdd/ReadVariableOp)^sequential/dense_2/MatMul/ReadVariableOp*
_output_shapes
 "!

identity_4Identity_4:output:0*(
_construction_contextkEagerRuntime*Q
_input_shapes@
>:���������:���������
: : : : : : : : : : 22
SGD/AssignAddVariableOpSGD/AssignAddVariableOp2>
SGD/StatefulPartitionedCall_1SGD/StatefulPartitionedCall_12>
SGD/StatefulPartitionedCall_2SGD/StatefulPartitionedCall_22>
SGD/StatefulPartitionedCall_3SGD/StatefulPartitionedCall_32:
SGD/StatefulPartitionedCallSGD/StatefulPartitionedCall2V
)sequential/dense_1/BiasAdd/ReadVariableOp)sequential/dense_1/BiasAdd/ReadVariableOp2T
(sequential/dense_1/MatMul/ReadVariableOp(sequential/dense_1/MatMul/ReadVariableOp2V
)sequential/dense_2/BiasAdd/ReadVariableOp)sequential/dense_2/BiasAdd/ReadVariableOp2T
(sequential/dense_2/MatMul/ReadVariableOp(sequential/dense_2/MatMul/ReadVariableOp:($
"
_user_specified_name
resource:%
!

_user_specified_name90930:%	!

_user_specified_name90912:%!

_user_specified_name90894:%!

_user_specified_name90876:%!

_user_specified_name90874:($
"
_user_specified_name
resource:($
"
_user_specified_name
resource:($
"
_user_specified_name
resource:($
"
_user_specified_name
resource:JF
'
_output_shapes
:���������


_user_specified_namey:N J
+
_output_shapes
:���������

_user_specified_namex
�
�
E__inference_sequential_layer_call_and_return_conditional_losses_91011
flatten_input!
dense_1_91000:
��
dense_1_91002:	� 
dense_2_91005:	�

dense_2_91007:

identity��dense_1/StatefulPartitionedCall�dense_2/StatefulPartitionedCall�
flatten/PartitionedCallPartitionedCallflatten_input*
Tin
2*
Tout
2*
_collective_manager_ids
 *(
_output_shapes
:����������* 
_read_only_resource_inputs
 *0
config_proto 

CPU

GPU2*0J 8� *K
fFRD
B__inference_flatten_layer_call_and_return_conditional_losses_90962�
dense_1/StatefulPartitionedCallStatefulPartitionedCall flatten/PartitionedCall:output:0dense_1_91000dense_1_91002*
Tin
2*
Tout
2*
_collective_manager_ids
 *(
_output_shapes
:����������*$
_read_only_resource_inputs
*0
config_proto 

CPU

GPU2*0J 8� *K
fFRD
B__inference_dense_1_layer_call_and_return_conditional_losses_90974�
dense_2/StatefulPartitionedCallStatefulPartitionedCall(dense_1/StatefulPartitionedCall:output:0dense_2_91005dense_2_91007*
Tin
2*
Tout
2*
_collective_manager_ids
 *'
_output_shapes
:���������
*$
_read_only_resource_inputs
*0
config_proto 

CPU

GPU2*0J 8� *K
fFRD
B__inference_dense_2_layer_call_and_return_conditional_losses_90989w
IdentityIdentity(dense_2/StatefulPartitionedCall:output:0^NoOp*
T0*'
_output_shapes
:���������
f
NoOpNoOp ^dense_1/StatefulPartitionedCall ^dense_2/StatefulPartitionedCall*
_output_shapes
 "
identityIdentity:output:0*(
_construction_contextkEagerRuntime*2
_input_shapes!
:���������: : : : 2B
dense_1/StatefulPartitionedCalldense_1/StatefulPartitionedCall2B
dense_2/StatefulPartitionedCalldense_2/StatefulPartitionedCall:%!

_user_specified_name91007:%!

_user_specified_name91005:%!

_user_specified_name91002:%!

_user_specified_name91000:Z V
+
_output_shapes
:���������
'
_user_specified_nameflatten_input
�
�
__inference_infer_90587
xE
1sequential_dense_1_matmul_readvariableop_resource:
��A
2sequential_dense_1_biasadd_readvariableop_resource:	�D
1sequential_dense_2_matmul_readvariableop_resource:	�
@
2sequential_dense_2_biasadd_readvariableop_resource:

identity

identity_1��)sequential/dense_1/BiasAdd/ReadVariableOp�(sequential/dense_1/MatMul/ReadVariableOp�)sequential/dense_2/BiasAdd/ReadVariableOp�(sequential/dense_2/MatMul/ReadVariableOpi
sequential/flatten/ConstConst*
_output_shapes
:*
dtype0*
valueB"����  ~
sequential/flatten/ReshapeReshapex!sequential/flatten/Const:output:0*
T0*(
_output_shapes
:�����������
(sequential/dense_1/MatMul/ReadVariableOpReadVariableOp1sequential_dense_1_matmul_readvariableop_resource* 
_output_shapes
:
��*
dtype0�
sequential/dense_1/MatMulMatMul#sequential/flatten/Reshape:output:00sequential/dense_1/MatMul/ReadVariableOp:value:0*
T0*(
_output_shapes
:�����������
)sequential/dense_1/BiasAdd/ReadVariableOpReadVariableOp2sequential_dense_1_biasadd_readvariableop_resource*
_output_shapes	
:�*
dtype0�
sequential/dense_1/BiasAddBiasAdd#sequential/dense_1/MatMul:product:01sequential/dense_1/BiasAdd/ReadVariableOp:value:0*
T0*(
_output_shapes
:����������w
sequential/dense_1/ReluRelu#sequential/dense_1/BiasAdd:output:0*
T0*(
_output_shapes
:�����������
(sequential/dense_2/MatMul/ReadVariableOpReadVariableOp1sequential_dense_2_matmul_readvariableop_resource*
_output_shapes
:	�
*
dtype0�
sequential/dense_2/MatMulMatMul%sequential/dense_1/Relu:activations:00sequential/dense_2/MatMul/ReadVariableOp:value:0*
T0*'
_output_shapes
:���������
�
)sequential/dense_2/BiasAdd/ReadVariableOpReadVariableOp2sequential_dense_2_biasadd_readvariableop_resource*
_output_shapes
:
*
dtype0�
sequential/dense_2/BiasAddBiasAdd#sequential/dense_2/MatMul:product:01sequential/dense_2/BiasAdd/ReadVariableOp:value:0*
T0*'
_output_shapes
:���������
i
SoftmaxSoftmax#sequential/dense_2/BiasAdd:output:0*
T0*'
_output_shapes
:���������
r
IdentityIdentity#sequential/dense_2/BiasAdd:output:0^NoOp*
T0*'
_output_shapes
:���������
b

Identity_1IdentitySoftmax:softmax:0^NoOp*
T0*'
_output_shapes
:���������
�
NoOpNoOp*^sequential/dense_1/BiasAdd/ReadVariableOp)^sequential/dense_1/MatMul/ReadVariableOp*^sequential/dense_2/BiasAdd/ReadVariableOp)^sequential/dense_2/MatMul/ReadVariableOp*
_output_shapes
 "!

identity_1Identity_1:output:0"
identityIdentity:output:0*(
_construction_contextkEagerRuntime*2
_input_shapes!
:���������: : : : 2V
)sequential/dense_1/BiasAdd/ReadVariableOp)sequential/dense_1/BiasAdd/ReadVariableOp2T
(sequential/dense_1/MatMul/ReadVariableOp(sequential/dense_1/MatMul/ReadVariableOp2V
)sequential/dense_2/BiasAdd/ReadVariableOp)sequential/dense_2/BiasAdd/ReadVariableOp2T
(sequential/dense_2/MatMul/ReadVariableOp(sequential/dense_2/MatMul/ReadVariableOp:($
"
_user_specified_name
resource:($
"
_user_specified_name
resource:($
"
_user_specified_name
resource:($
"
_user_specified_name
resource:N J
+
_output_shapes
:���������

_user_specified_namex
�
�
__inference_infer_90704
xE
1sequential_dense_1_matmul_readvariableop_resource:
��A
2sequential_dense_1_biasadd_readvariableop_resource:	�D
1sequential_dense_2_matmul_readvariableop_resource:	�
@
2sequential_dense_2_biasadd_readvariableop_resource:

identity

identity_1��)sequential/dense_1/BiasAdd/ReadVariableOp�(sequential/dense_1/MatMul/ReadVariableOp�)sequential/dense_2/BiasAdd/ReadVariableOp�(sequential/dense_2/MatMul/ReadVariableOpi
sequential/flatten/ConstConst*
_output_shapes
:*
dtype0*
valueB"����  ~
sequential/flatten/ReshapeReshapex!sequential/flatten/Const:output:0*
T0*(
_output_shapes
:�����������
(sequential/dense_1/MatMul/ReadVariableOpReadVariableOp1sequential_dense_1_matmul_readvariableop_resource* 
_output_shapes
:
��*
dtype0�
sequential/dense_1/MatMulMatMul#sequential/flatten/Reshape:output:00sequential/dense_1/MatMul/ReadVariableOp:value:0*
T0*(
_output_shapes
:�����������
)sequential/dense_1/BiasAdd/ReadVariableOpReadVariableOp2sequential_dense_1_biasadd_readvariableop_resource*
_output_shapes	
:�*
dtype0�
sequential/dense_1/BiasAddBiasAdd#sequential/dense_1/MatMul:product:01sequential/dense_1/BiasAdd/ReadVariableOp:value:0*
T0*(
_output_shapes
:����������w
sequential/dense_1/ReluRelu#sequential/dense_1/BiasAdd:output:0*
T0*(
_output_shapes
:�����������
(sequential/dense_2/MatMul/ReadVariableOpReadVariableOp1sequential_dense_2_matmul_readvariableop_resource*
_output_shapes
:	�
*
dtype0�
sequential/dense_2/MatMulMatMul%sequential/dense_1/Relu:activations:00sequential/dense_2/MatMul/ReadVariableOp:value:0*
T0*'
_output_shapes
:���������
�
)sequential/dense_2/BiasAdd/ReadVariableOpReadVariableOp2sequential_dense_2_biasadd_readvariableop_resource*
_output_shapes
:
*
dtype0�
sequential/dense_2/BiasAddBiasAdd#sequential/dense_2/MatMul:product:01sequential/dense_2/BiasAdd/ReadVariableOp:value:0*
T0*'
_output_shapes
:���������
i
SoftmaxSoftmax#sequential/dense_2/BiasAdd:output:0*
T0*'
_output_shapes
:���������
r
IdentityIdentity#sequential/dense_2/BiasAdd:output:0^NoOp*
T0*'
_output_shapes
:���������
b

Identity_1IdentitySoftmax:softmax:0^NoOp*
T0*'
_output_shapes
:���������
�
NoOpNoOp*^sequential/dense_1/BiasAdd/ReadVariableOp)^sequential/dense_1/MatMul/ReadVariableOp*^sequential/dense_2/BiasAdd/ReadVariableOp)^sequential/dense_2/MatMul/ReadVariableOp*
_output_shapes
 "!

identity_1Identity_1:output:0"
identityIdentity:output:0*(
_construction_contextkEagerRuntime*2
_input_shapes!
:���������: : : : 2V
)sequential/dense_1/BiasAdd/ReadVariableOp)sequential/dense_1/BiasAdd/ReadVariableOp2T
(sequential/dense_1/MatMul/ReadVariableOp(sequential/dense_1/MatMul/ReadVariableOp2V
)sequential/dense_2/BiasAdd/ReadVariableOp)sequential/dense_2/BiasAdd/ReadVariableOp2T
(sequential/dense_2/MatMul/ReadVariableOp(sequential/dense_2/MatMul/ReadVariableOp:($
"
_user_specified_name
resource:($
"
_user_specified_name
resource:($
"
_user_specified_name
resource:($
"
_user_specified_name
resource:N J
+
_output_shapes
:���������

_user_specified_namex
�
�
__inference_save_90740
checkpoint_path0
read_readvariableop_resource:
��-
read_1_readvariableop_resource:	�1
read_2_readvariableop_resource:	�
,
read_3_readvariableop_resource:


identity_4��Read/ReadVariableOp�Read_1/ReadVariableOp�Read_2/ReadVariableOp�Read_3/ReadVariableOp�saver
Read/ReadVariableOpReadVariableOpread_readvariableop_resource* 
_output_shapes
:
��*
dtype0\
IdentityIdentityRead/ReadVariableOp:value:0*
T0* 
_output_shapes
:
��q
Read_1/ReadVariableOpReadVariableOpread_1_readvariableop_resource*
_output_shapes	
:�*
dtype0[

Identity_1IdentityRead_1/ReadVariableOp:value:0*
T0*
_output_shapes	
:�u
Read_2/ReadVariableOpReadVariableOpread_2_readvariableop_resource*
_output_shapes
:	�
*
dtype0_

Identity_2IdentityRead_2/ReadVariableOp:value:0*
T0*
_output_shapes
:	�
p
Read_3/ReadVariableOpReadVariableOpread_3_readvariableop_resource*
_output_shapes
:
*
dtype0Z

Identity_3IdentityRead_3/ReadVariableOp:value:0*
T0*
_output_shapes
:
�
save/tensor_namesConst*
_output_shapes
:*
dtype0*W
valueNBLBdense_1/kernel:0Bdense_1/bias:0Bdense_2/kernel:0Bdense_2/bias:0�
saveSavecheckpoint_pathsave/tensor_names:output:0Identity:output:0Identity_1:output:0Identity_2:output:0Identity_3:output:0*
T
2*
_output_shapes
 O

Identity_4Identitycheckpoint_path^NoOp*
T0*
_output_shapes
: �
NoOpNoOp^Read/ReadVariableOp^Read_1/ReadVariableOp^Read_2/ReadVariableOp^Read_3/ReadVariableOp^save*
_output_shapes
 "!

identity_4Identity_4:output:0*(
_construction_contextkEagerRuntime*
_input_shapes

: : : : : 2*
Read/ReadVariableOpRead/ReadVariableOp2.
Read_1/ReadVariableOpRead_1/ReadVariableOp2.
Read_2/ReadVariableOpRead_2/ReadVariableOp2.
Read_3/ReadVariableOpRead_3/ReadVariableOp2
savesave:($
"
_user_specified_name
resource:($
"
_user_specified_name
resource:($
"
_user_specified_name
resource:($
"
_user_specified_name
resource:G C

_output_shapes
: 
)
_user_specified_namecheckpoint_path
�
O
"__inference__update_step_xla_91077
gradient
variable:	�
*
_XlaMustCompile(*(
_construction_contextkEagerRuntime* 
_input_shapes
:	�
: *
	_noinline(:($
"
_user_specified_name
variable:I E

_output_shapes
:	�

"
_user_specified_name
gradient
�
^
B__inference_flatten_layer_call_and_return_conditional_losses_90962

inputs
identityV
ConstConst*
_output_shapes
:*
dtype0*
valueB"����  ]
ReshapeReshapeinputsConst:output:0*
T0*(
_output_shapes
:����������Y
IdentityIdentityReshape:output:0*
T0*(
_output_shapes
:����������"
identityIdentity:output:0*(
_construction_contextkEagerRuntime**
_input_shapes
:���������:S O
+
_output_shapes
:���������
 
_user_specified_nameinputs
�
�
"__inference__update_step_xla_90891
gradient
variable:	�%
mul_readvariableop_resource: &
readvariableop_resource:	���AssignAddVariableOp�AssignVariableOp�ReadVariableOp�ReadVariableOp_1�mul/ReadVariableOpK
Cast/xConst*
_output_shapes
: *
dtype0*
valueB
 *    :
NegNeggradient*
T0*
_output_shapes	
:�f
mul/ReadVariableOpReadVariableOpmul_readvariableop_resource*
_output_shapes
: *
dtype0U
mulMulNeg:y:0mul/ReadVariableOp:value:0*
T0*
_output_shapes	
:�c
ReadVariableOpReadVariableOpreadvariableop_resource*
_output_shapes	
:�*
dtype0[
mul_1MulReadVariableOp:value:0Cast/x:output:0*
T0*
_output_shapes	
:�F
addAddV2mul:z:0	mul_1:z:0*
T0*
_output_shapes	
:��
AssignVariableOpAssignVariableOpreadvariableop_resourceadd:z:0^ReadVariableOp*
_output_shapes
 *
dtype0*
validate_shape(x
ReadVariableOp_1ReadVariableOpreadvariableop_resource^AssignVariableOp*
_output_shapes	
:�*
dtype0q
AssignAddVariableOpAssignAddVariableOpvariableReadVariableOp_1:value:0*
_output_shapes
 *
dtype0*
_XlaMustCompile(*(
_construction_contextkEagerRuntime* 
_input_shapes
:�: : : *
	_noinline(2*
AssignAddVariableOpAssignAddVariableOp2$
AssignVariableOpAssignVariableOp2$
ReadVariableOp_1ReadVariableOp_12 
ReadVariableOpReadVariableOp2(
mul/ReadVariableOpmul/ReadVariableOp:($
"
_user_specified_name
resource:($
"
_user_specified_name
resource:($
"
_user_specified_name
variable:E A

_output_shapes	
:�
"
_user_specified_name
gradient
�
�
"__inference__update_step_xla_90927
gradient
variable:
%
mul_readvariableop_resource: %
readvariableop_resource:
��AssignAddVariableOp�AssignVariableOp�ReadVariableOp�ReadVariableOp_1�mul/ReadVariableOpK
Cast/xConst*
_output_shapes
: *
dtype0*
valueB
 *    9
NegNeggradient*
T0*
_output_shapes
:
f
mul/ReadVariableOpReadVariableOpmul_readvariableop_resource*
_output_shapes
: *
dtype0T
mulMulNeg:y:0mul/ReadVariableOp:value:0*
T0*
_output_shapes
:
b
ReadVariableOpReadVariableOpreadvariableop_resource*
_output_shapes
:
*
dtype0Z
mul_1MulReadVariableOp:value:0Cast/x:output:0*
T0*
_output_shapes
:
E
addAddV2mul:z:0	mul_1:z:0*
T0*
_output_shapes
:
�
AssignVariableOpAssignVariableOpreadvariableop_resourceadd:z:0^ReadVariableOp*
_output_shapes
 *
dtype0*
validate_shape(w
ReadVariableOp_1ReadVariableOpreadvariableop_resource^AssignVariableOp*
_output_shapes
:
*
dtype0q
AssignAddVariableOpAssignAddVariableOpvariableReadVariableOp_1:value:0*
_output_shapes
 *
dtype0*
_XlaMustCompile(*(
_construction_contextkEagerRuntime*
_input_shapes
:
: : : *
	_noinline(2*
AssignAddVariableOpAssignAddVariableOp2$
AssignVariableOpAssignVariableOp2$
ReadVariableOp_1ReadVariableOp_12 
ReadVariableOpReadVariableOp2(
mul/ReadVariableOpmul/ReadVariableOp:($
"
_user_specified_name
resource:($
"
_user_specified_name
resource:($
"
_user_specified_name
variable:D @

_output_shapes
:

"
_user_specified_name
gradient
�
�
__inference_restore_90606
checkpoint_path-
assignvariableop_resource:
��*
assignvariableop_1_resource:	�.
assignvariableop_2_resource:	�
)
assignvariableop_3_resource:

identity

identity_1

identity_2

identity_3��AssignVariableOp�AssignVariableOp_1�AssignVariableOp_2�AssignVariableOp_3�restore�	restore_1�	restore_2�	restore_3d
restore/tensor_nameConst*
_output_shapes
: *
dtype0*!
valueB Bdense_1/kernel:0e
restoreRestorecheckpoint_pathrestore/tensor_name:output:0*
_output_shapes
:*
dt0�
AssignVariableOpAssignVariableOpassignvariableop_resourcerestore:tensor:0*
_output_shapes
 *
dtype0*
validate_shape(d
restore_1/tensor_nameConst*
_output_shapes
: *
dtype0*
valueB Bdense_1/bias:0s
	restore_1Restorecheckpoint_pathrestore_1/tensor_name:output:0^restore*
_output_shapes
:*
dt0�
AssignVariableOp_1AssignVariableOpassignvariableop_1_resourcerestore_1:tensor:0*
_output_shapes
 *
dtype0*
validate_shape(f
restore_2/tensor_nameConst*
_output_shapes
: *
dtype0*!
valueB Bdense_2/kernel:0u
	restore_2Restorecheckpoint_pathrestore_2/tensor_name:output:0
^restore_1*
_output_shapes
:*
dt0�
AssignVariableOp_2AssignVariableOpassignvariableop_2_resourcerestore_2:tensor:0*
_output_shapes
 *
dtype0*
validate_shape(d
restore_3/tensor_nameConst*
_output_shapes
: *
dtype0*
valueB Bdense_2/bias:0u
	restore_3Restorecheckpoint_pathrestore_3/tensor_name:output:0
^restore_2*
_output_shapes
:*
dt0�
AssignVariableOp_3AssignVariableOpassignvariableop_3_resourcerestore_3:tensor:0*
_output_shapes
 *
dtype0*
validate_shape(R
IdentityIdentityrestore_1:tensor:0^NoOp*
T0*
_output_shapes
:R

Identity_1Identityrestore:tensor:0^NoOp*
T0*
_output_shapes
:T

Identity_2Identityrestore_3:tensor:0^NoOp*
T0*
_output_shapes
:T

Identity_3Identityrestore_2:tensor:0^NoOp*
T0*
_output_shapes
:�
NoOpNoOp^AssignVariableOp^AssignVariableOp_1^AssignVariableOp_2^AssignVariableOp_3^restore
^restore_1
^restore_2
^restore_3*
_output_shapes
 "!

identity_1Identity_1:output:0"!

identity_2Identity_2:output:0"!

identity_3Identity_3:output:0"
identityIdentity:output:0*(
_construction_contextkEagerRuntime*
_input_shapes

: : : : : 2(
AssignVariableOp_1AssignVariableOp_12(
AssignVariableOp_2AssignVariableOp_22(
AssignVariableOp_3AssignVariableOp_32$
AssignVariableOpAssignVariableOp2
	restore_1	restore_12
	restore_2	restore_22
	restore_3	restore_32
restorerestore:($
"
_user_specified_name
resource:($
"
_user_specified_name
resource:($
"
_user_specified_name
resource:($
"
_user_specified_name
resource:G C

_output_shapes
: 
)
_user_specified_namecheckpoint_path
��
�
__inference_train_449
x
yE
1sequential_dense_1_matmul_readvariableop_resource:
��A
2sequential_dense_1_biasadd_readvariableop_resource:	�D
1sequential_dense_2_matmul_readvariableop_resource:	�
@
2sequential_dense_2_biasadd_readvariableop_resource:

sgd_430: 
sgd_432:
��
sgd_436:	�
sgd_440:	�

sgd_444:
*
 sgd_assignaddvariableop_resource:	 

identity_4��SGD/AssignAddVariableOp�SGD/StatefulPartitionedCall�SGD/StatefulPartitionedCall_1�SGD/StatefulPartitionedCall_2�SGD/StatefulPartitionedCall_3�)sequential/dense_1/BiasAdd/ReadVariableOp�(sequential/dense_1/MatMul/ReadVariableOp�)sequential/dense_2/BiasAdd/ReadVariableOp�(sequential/dense_2/MatMul/ReadVariableOpi
sequential/flatten/ConstConst*
_output_shapes
:*
dtype0*
valueB"����  ~
sequential/flatten/ReshapeReshapex!sequential/flatten/Const:output:0*
T0*(
_output_shapes
:�����������
(sequential/dense_1/MatMul/ReadVariableOpReadVariableOp1sequential_dense_1_matmul_readvariableop_resource* 
_output_shapes
:
��*
dtype0�
sequential/dense_1/MatMulMatMul#sequential/flatten/Reshape:output:00sequential/dense_1/MatMul/ReadVariableOp:value:0*
T0*(
_output_shapes
:�����������
)sequential/dense_1/BiasAdd/ReadVariableOpReadVariableOp2sequential_dense_1_biasadd_readvariableop_resource*
_output_shapes	
:�*
dtype0�
sequential/dense_1/BiasAddBiasAdd#sequential/dense_1/MatMul:product:01sequential/dense_1/BiasAdd/ReadVariableOp:value:0*
T0*(
_output_shapes
:����������w
sequential/dense_1/ReluRelu#sequential/dense_1/BiasAdd:output:0*
T0*(
_output_shapes
:�����������
(sequential/dense_2/MatMul/ReadVariableOpReadVariableOp1sequential_dense_2_matmul_readvariableop_resource*
_output_shapes
:	�
*
dtype0�
sequential/dense_2/MatMulMatMul%sequential/dense_1/Relu:activations:00sequential/dense_2/MatMul/ReadVariableOp:value:0*
T0*'
_output_shapes
:���������
�
)sequential/dense_2/BiasAdd/ReadVariableOpReadVariableOp2sequential_dense_2_biasadd_readvariableop_resource*
_output_shapes
:
*
dtype0�
sequential/dense_2/BiasAddBiasAdd#sequential/dense_2/MatMul:product:01sequential/dense_2/BiasAdd/ReadVariableOp:value:0*
T0*'
_output_shapes
:���������
c
categorical_crossentropy/ConstConst*
_output_shapes
: *
dtype0*
valueB
 *    �
?categorical_crossentropy/softmax_cross_entropy_with_logits/RankConst*
_output_shapes
: *
dtype0*
value	B :�
@categorical_crossentropy/softmax_cross_entropy_with_logits/ShapeShape#sequential/dense_2/BiasAdd:output:0*
T0*
_output_shapes
::���
Acategorical_crossentropy/softmax_cross_entropy_with_logits/Rank_1Const*
_output_shapes
: *
dtype0*
value	B :�
Bcategorical_crossentropy/softmax_cross_entropy_with_logits/Shape_1Shape#sequential/dense_2/BiasAdd:output:0*
T0*
_output_shapes
::���
@categorical_crossentropy/softmax_cross_entropy_with_logits/Sub/yConst*
_output_shapes
: *
dtype0*
value	B :�
>categorical_crossentropy/softmax_cross_entropy_with_logits/SubSubJcategorical_crossentropy/softmax_cross_entropy_with_logits/Rank_1:output:0Icategorical_crossentropy/softmax_cross_entropy_with_logits/Sub/y:output:0*
T0*
_output_shapes
: �
Fcategorical_crossentropy/softmax_cross_entropy_with_logits/Slice/beginPackBcategorical_crossentropy/softmax_cross_entropy_with_logits/Sub:z:0*
N*
T0*
_output_shapes
:�
Ecategorical_crossentropy/softmax_cross_entropy_with_logits/Slice/sizeConst*
_output_shapes
:*
dtype0*
valueB:�
@categorical_crossentropy/softmax_cross_entropy_with_logits/SliceSliceKcategorical_crossentropy/softmax_cross_entropy_with_logits/Shape_1:output:0Ocategorical_crossentropy/softmax_cross_entropy_with_logits/Slice/begin:output:0Ncategorical_crossentropy/softmax_cross_entropy_with_logits/Slice/size:output:0*
Index0*
T0*
_output_shapes
:�
Jcategorical_crossentropy/softmax_cross_entropy_with_logits/concat/values_0Const*
_output_shapes
:*
dtype0*
valueB:
����������
Fcategorical_crossentropy/softmax_cross_entropy_with_logits/concat/axisConst*
_output_shapes
: *
dtype0*
value	B : �
Acategorical_crossentropy/softmax_cross_entropy_with_logits/concatConcatV2Scategorical_crossentropy/softmax_cross_entropy_with_logits/concat/values_0:output:0Icategorical_crossentropy/softmax_cross_entropy_with_logits/Slice:output:0Ocategorical_crossentropy/softmax_cross_entropy_with_logits/concat/axis:output:0*
N*
T0*
_output_shapes
:�
Bcategorical_crossentropy/softmax_cross_entropy_with_logits/ReshapeReshape#sequential/dense_2/BiasAdd:output:0Jcategorical_crossentropy/softmax_cross_entropy_with_logits/concat:output:0*
T0*0
_output_shapes
:�������������������
Acategorical_crossentropy/softmax_cross_entropy_with_logits/Rank_2Const*
_output_shapes
: *
dtype0*
value	B :�
Bcategorical_crossentropy/softmax_cross_entropy_with_logits/Shape_2Shapey*
T0*
_output_shapes
::���
Bcategorical_crossentropy/softmax_cross_entropy_with_logits/Sub_1/yConst*
_output_shapes
: *
dtype0*
value	B :�
@categorical_crossentropy/softmax_cross_entropy_with_logits/Sub_1SubJcategorical_crossentropy/softmax_cross_entropy_with_logits/Rank_2:output:0Kcategorical_crossentropy/softmax_cross_entropy_with_logits/Sub_1/y:output:0*
T0*
_output_shapes
: �
Hcategorical_crossentropy/softmax_cross_entropy_with_logits/Slice_1/beginPackDcategorical_crossentropy/softmax_cross_entropy_with_logits/Sub_1:z:0*
N*
T0*
_output_shapes
:�
Gcategorical_crossentropy/softmax_cross_entropy_with_logits/Slice_1/sizeConst*
_output_shapes
:*
dtype0*
valueB:�
Bcategorical_crossentropy/softmax_cross_entropy_with_logits/Slice_1SliceKcategorical_crossentropy/softmax_cross_entropy_with_logits/Shape_2:output:0Qcategorical_crossentropy/softmax_cross_entropy_with_logits/Slice_1/begin:output:0Pcategorical_crossentropy/softmax_cross_entropy_with_logits/Slice_1/size:output:0*
Index0*
T0*
_output_shapes
:�
Lcategorical_crossentropy/softmax_cross_entropy_with_logits/concat_1/values_0Const*
_output_shapes
:*
dtype0*
valueB:
����������
Hcategorical_crossentropy/softmax_cross_entropy_with_logits/concat_1/axisConst*
_output_shapes
: *
dtype0*
value	B : �
Ccategorical_crossentropy/softmax_cross_entropy_with_logits/concat_1ConcatV2Ucategorical_crossentropy/softmax_cross_entropy_with_logits/concat_1/values_0:output:0Kcategorical_crossentropy/softmax_cross_entropy_with_logits/Slice_1:output:0Qcategorical_crossentropy/softmax_cross_entropy_with_logits/concat_1/axis:output:0*
N*
T0*
_output_shapes
:�
Dcategorical_crossentropy/softmax_cross_entropy_with_logits/Reshape_1ReshapeyLcategorical_crossentropy/softmax_cross_entropy_with_logits/concat_1:output:0*
T0*0
_output_shapes
:�������������������
:categorical_crossentropy/softmax_cross_entropy_with_logitsSoftmaxCrossEntropyWithLogitsKcategorical_crossentropy/softmax_cross_entropy_with_logits/Reshape:output:0Mcategorical_crossentropy/softmax_cross_entropy_with_logits/Reshape_1:output:0*
T0*?
_output_shapes-
+:���������:�������������������
Bcategorical_crossentropy/softmax_cross_entropy_with_logits/Sub_2/yConst*
_output_shapes
: *
dtype0*
value	B :�
@categorical_crossentropy/softmax_cross_entropy_with_logits/Sub_2SubHcategorical_crossentropy/softmax_cross_entropy_with_logits/Rank:output:0Kcategorical_crossentropy/softmax_cross_entropy_with_logits/Sub_2/y:output:0*
T0*
_output_shapes
: �
Hcategorical_crossentropy/softmax_cross_entropy_with_logits/Slice_2/beginConst*
_output_shapes
:*
dtype0*
valueB: �
Gcategorical_crossentropy/softmax_cross_entropy_with_logits/Slice_2/sizePackDcategorical_crossentropy/softmax_cross_entropy_with_logits/Sub_2:z:0*
N*
T0*
_output_shapes
:�
Bcategorical_crossentropy/softmax_cross_entropy_with_logits/Slice_2SliceIcategorical_crossentropy/softmax_cross_entropy_with_logits/Shape:output:0Qcategorical_crossentropy/softmax_cross_entropy_with_logits/Slice_2/begin:output:0Pcategorical_crossentropy/softmax_cross_entropy_with_logits/Slice_2/size:output:0*
Index0*
T0*
_output_shapes
:�
Dcategorical_crossentropy/softmax_cross_entropy_with_logits/Reshape_2ReshapeAcategorical_crossentropy/softmax_cross_entropy_with_logits:loss:0Kcategorical_crossentropy/softmax_cross_entropy_with_logits/Slice_2:output:0*
T0*#
_output_shapes
:���������q
,categorical_crossentropy/weighted_loss/ConstConst*
_output_shapes
: *
dtype0*
valueB
 *  �?�
*categorical_crossentropy/weighted_loss/MulMulMcategorical_crossentropy/softmax_cross_entropy_with_logits/Reshape_2:output:05categorical_crossentropy/weighted_loss/Const:output:0*
T0*#
_output_shapes
:���������x
.categorical_crossentropy/weighted_loss/Const_1Const*
_output_shapes
:*
dtype0*
valueB: �
*categorical_crossentropy/weighted_loss/SumSum.categorical_crossentropy/weighted_loss/Mul:z:07categorical_crossentropy/weighted_loss/Const_1:output:0*
T0*
_output_shapes
: �
3categorical_crossentropy/weighted_loss/num_elementsSize.categorical_crossentropy/weighted_loss/Mul:z:0*
T0*
_output_shapes
: �
8categorical_crossentropy/weighted_loss/num_elements/CastCast<categorical_crossentropy/weighted_loss/num_elements:output:0*

DstT0*

SrcT0*
_output_shapes
: m
+categorical_crossentropy/weighted_loss/RankConst*
_output_shapes
: *
dtype0*
value	B : t
2categorical_crossentropy/weighted_loss/range/startConst*
_output_shapes
: *
dtype0*
value	B : t
2categorical_crossentropy/weighted_loss/range/deltaConst*
_output_shapes
: *
dtype0*
value	B :�
,categorical_crossentropy/weighted_loss/rangeRange;categorical_crossentropy/weighted_loss/range/start:output:04categorical_crossentropy/weighted_loss/Rank:output:0;categorical_crossentropy/weighted_loss/range/delta:output:0*
_output_shapes
: �
,categorical_crossentropy/weighted_loss/Sum_1Sum3categorical_crossentropy/weighted_loss/Sum:output:05categorical_crossentropy/weighted_loss/range:output:0*
T0*
_output_shapes
: �
,categorical_crossentropy/weighted_loss/valueDivNoNan5categorical_crossentropy/weighted_loss/Sum_1:output:0<categorical_crossentropy/weighted_loss/num_elements/Cast:y:0*
T0*
_output_shapes
: I
onesConst*
_output_shapes
: *
dtype0*
valueB
 *  �?�
@gradient_tape/categorical_crossentropy/weighted_loss/value/ShapeConst*
_output_shapes
: *
dtype0*
valueB �
Bgradient_tape/categorical_crossentropy/weighted_loss/value/Shape_1Const*
_output_shapes
: *
dtype0*
valueB �
Pgradient_tape/categorical_crossentropy/weighted_loss/value/BroadcastGradientArgsBroadcastGradientArgsIgradient_tape/categorical_crossentropy/weighted_loss/value/Shape:output:0Kgradient_tape/categorical_crossentropy/weighted_loss/value/Shape_1:output:0*2
_output_shapes 
:���������:����������
Egradient_tape/categorical_crossentropy/weighted_loss/value/div_no_nanDivNoNanones:output:0<categorical_crossentropy/weighted_loss/num_elements/Cast:y:0*
T0*
_output_shapes
: �
>gradient_tape/categorical_crossentropy/weighted_loss/value/SumSumIgradient_tape/categorical_crossentropy/weighted_loss/value/div_no_nan:z:0Ugradient_tape/categorical_crossentropy/weighted_loss/value/BroadcastGradientArgs:r0:0*
T0*
_output_shapes
: �
Bgradient_tape/categorical_crossentropy/weighted_loss/value/ReshapeReshapeGgradient_tape/categorical_crossentropy/weighted_loss/value/Sum:output:0Igradient_tape/categorical_crossentropy/weighted_loss/value/Shape:output:0*
T0*
_output_shapes
: �
>gradient_tape/categorical_crossentropy/weighted_loss/value/NegNeg5categorical_crossentropy/weighted_loss/Sum_1:output:0*
T0*
_output_shapes
: �
Ggradient_tape/categorical_crossentropy/weighted_loss/value/div_no_nan_1DivNoNanBgradient_tape/categorical_crossentropy/weighted_loss/value/Neg:y:0<categorical_crossentropy/weighted_loss/num_elements/Cast:y:0*
T0*
_output_shapes
: �
Ggradient_tape/categorical_crossentropy/weighted_loss/value/div_no_nan_2DivNoNanKgradient_tape/categorical_crossentropy/weighted_loss/value/div_no_nan_1:z:0<categorical_crossentropy/weighted_loss/num_elements/Cast:y:0*
T0*
_output_shapes
: �
>gradient_tape/categorical_crossentropy/weighted_loss/value/mulMulones:output:0Kgradient_tape/categorical_crossentropy/weighted_loss/value/div_no_nan_2:z:0*
T0*
_output_shapes
: �
@gradient_tape/categorical_crossentropy/weighted_loss/value/Sum_1SumBgradient_tape/categorical_crossentropy/weighted_loss/value/mul:z:0Ugradient_tape/categorical_crossentropy/weighted_loss/value/BroadcastGradientArgs:r1:0*
T0*
_output_shapes
: �
Dgradient_tape/categorical_crossentropy/weighted_loss/value/Reshape_1ReshapeIgradient_tape/categorical_crossentropy/weighted_loss/value/Sum_1:output:0Kgradient_tape/categorical_crossentropy/weighted_loss/value/Shape_1:output:0*
T0*
_output_shapes
: �
Bgradient_tape/categorical_crossentropy/weighted_loss/Reshape/shapeConst*
_output_shapes
: *
dtype0*
valueB �
Dgradient_tape/categorical_crossentropy/weighted_loss/Reshape/shape_1Const*
_output_shapes
: *
dtype0*
valueB �
<gradient_tape/categorical_crossentropy/weighted_loss/ReshapeReshapeKgradient_tape/categorical_crossentropy/weighted_loss/value/Reshape:output:0Mgradient_tape/categorical_crossentropy/weighted_loss/Reshape/shape_1:output:0*
T0*
_output_shapes
: }
:gradient_tape/categorical_crossentropy/weighted_loss/ConstConst*
_output_shapes
: *
dtype0*
valueB �
9gradient_tape/categorical_crossentropy/weighted_loss/TileTileEgradient_tape/categorical_crossentropy/weighted_loss/Reshape:output:0Cgradient_tape/categorical_crossentropy/weighted_loss/Const:output:0*
T0*
_output_shapes
: �
Dgradient_tape/categorical_crossentropy/weighted_loss/Reshape_1/shapeConst*
_output_shapes
:*
dtype0*
valueB:�
>gradient_tape/categorical_crossentropy/weighted_loss/Reshape_1ReshapeBgradient_tape/categorical_crossentropy/weighted_loss/Tile:output:0Mgradient_tape/categorical_crossentropy/weighted_loss/Reshape_1/shape:output:0*
T0*
_output_shapes
:�
:gradient_tape/categorical_crossentropy/weighted_loss/ShapeShape.categorical_crossentropy/weighted_loss/Mul:z:0*
T0*
_output_shapes
::���
;gradient_tape/categorical_crossentropy/weighted_loss/Tile_1TileGgradient_tape/categorical_crossentropy/weighted_loss/Reshape_1:output:0Cgradient_tape/categorical_crossentropy/weighted_loss/Shape:output:0*
T0*#
_output_shapes
:����������
8gradient_tape/categorical_crossentropy/weighted_loss/MulMulDgradient_tape/categorical_crossentropy/weighted_loss/Tile_1:output:05categorical_crossentropy/weighted_loss/Const:output:0*
T0*#
_output_shapes
:����������
Ngradient_tape/categorical_crossentropy/softmax_cross_entropy_with_logits/ShapeShapeAcategorical_crossentropy/softmax_cross_entropy_with_logits:loss:0*
T0*
_output_shapes
::���
Pgradient_tape/categorical_crossentropy/softmax_cross_entropy_with_logits/ReshapeReshape<gradient_tape/categorical_crossentropy/weighted_loss/Mul:z:0Wgradient_tape/categorical_crossentropy/softmax_cross_entropy_with_logits/Shape:output:0*
T0*#
_output_shapes
:����������
Wgradient_tape/categorical_crossentropy/softmax_cross_entropy_with_logits/ExpandDims/dimConst*
_output_shapes
: *
dtype0*
valueB :
����������
Sgradient_tape/categorical_crossentropy/softmax_cross_entropy_with_logits/ExpandDims
ExpandDimsYgradient_tape/categorical_crossentropy/softmax_cross_entropy_with_logits/Reshape:output:0`gradient_tape/categorical_crossentropy/softmax_cross_entropy_with_logits/ExpandDims/dim:output:0*
T0*'
_output_shapes
:����������
Lgradient_tape/categorical_crossentropy/softmax_cross_entropy_with_logits/mulMul\gradient_tape/categorical_crossentropy/softmax_cross_entropy_with_logits/ExpandDims:output:0Ecategorical_crossentropy/softmax_cross_entropy_with_logits:backprop:0*
T0*0
_output_shapes
:�������������������
Sgradient_tape/categorical_crossentropy/softmax_cross_entropy_with_logits/LogSoftmax
LogSoftmaxKcategorical_crossentropy/softmax_cross_entropy_with_logits/Reshape:output:0*
T0*0
_output_shapes
:�������������������
Lgradient_tape/categorical_crossentropy/softmax_cross_entropy_with_logits/NegNeg`gradient_tape/categorical_crossentropy/softmax_cross_entropy_with_logits/LogSoftmax:logsoftmax:0*
T0*0
_output_shapes
:�������������������
Ygradient_tape/categorical_crossentropy/softmax_cross_entropy_with_logits/ExpandDims_1/dimConst*
_output_shapes
: *
dtype0*
valueB :
����������
Ugradient_tape/categorical_crossentropy/softmax_cross_entropy_with_logits/ExpandDims_1
ExpandDimsYgradient_tape/categorical_crossentropy/softmax_cross_entropy_with_logits/Reshape:output:0bgradient_tape/categorical_crossentropy/softmax_cross_entropy_with_logits/ExpandDims_1/dim:output:0*
T0*'
_output_shapes
:����������
Ngradient_tape/categorical_crossentropy/softmax_cross_entropy_with_logits/mul_1Mul^gradient_tape/categorical_crossentropy/softmax_cross_entropy_with_logits/ExpandDims_1:output:0Pgradient_tape/categorical_crossentropy/softmax_cross_entropy_with_logits/Neg:y:0*
T0*0
_output_shapes
:�������������������
Pgradient_tape/categorical_crossentropy/softmax_cross_entropy_with_logits/Shape_1Shape#sequential/dense_2/BiasAdd:output:0*
T0*
_output_shapes
::���
Rgradient_tape/categorical_crossentropy/softmax_cross_entropy_with_logits/Reshape_1ReshapePgradient_tape/categorical_crossentropy/softmax_cross_entropy_with_logits/mul:z:0Ygradient_tape/categorical_crossentropy/softmax_cross_entropy_with_logits/Shape_1:output:0*
T0*'
_output_shapes
:���������
�
4gradient_tape/sequential/dense_2/BiasAdd/BiasAddGradBiasAddGrad[gradient_tape/categorical_crossentropy/softmax_cross_entropy_with_logits/Reshape_1:output:0*
T0*
_output_shapes
:
�
.gradient_tape/sequential/dense_2/MatMul/MatMulMatMul[gradient_tape/categorical_crossentropy/softmax_cross_entropy_with_logits/Reshape_1:output:00sequential/dense_2/MatMul/ReadVariableOp:value:0*
T0*(
_output_shapes
:����������*
transpose_b(�
0gradient_tape/sequential/dense_2/MatMul/MatMul_1MatMul%sequential/dense_1/Relu:activations:0[gradient_tape/categorical_crossentropy/softmax_cross_entropy_with_logits/Reshape_1:output:0*
T0*
_output_shapes
:	�
*
transpose_a(�
)gradient_tape/sequential/dense_1/ReluGradReluGrad8gradient_tape/sequential/dense_2/MatMul/MatMul:product:0%sequential/dense_1/Relu:activations:0*
T0*(
_output_shapes
:�����������
4gradient_tape/sequential/dense_1/BiasAdd/BiasAddGradBiasAddGrad5gradient_tape/sequential/dense_1/ReluGrad:backprops:0*
T0*
_output_shapes	
:��
.gradient_tape/sequential/dense_1/MatMul/MatMulMatMul#sequential/flatten/Reshape:output:05gradient_tape/sequential/dense_1/ReluGrad:backprops:0*
T0* 
_output_shapes
:
��*
transpose_a(y
IdentityIdentity8gradient_tape/sequential/dense_1/MatMul/MatMul:product:0*
T0* 
_output_shapes
:
��{

Identity_1Identity=gradient_tape/sequential/dense_1/BiasAdd/BiasAddGrad:output:0*
T0*
_output_shapes	
:�|

Identity_2Identity:gradient_tape/sequential/dense_2/MatMul/MatMul_1:product:0*
T0*
_output_shapes
:	�
z

Identity_3Identity=gradient_tape/sequential/dense_2/BiasAdd/BiasAddGrad:output:0*
T0*
_output_shapes
:
�
	IdentityN	IdentityN8gradient_tape/sequential/dense_1/MatMul/MatMul:product:0=gradient_tape/sequential/dense_1/BiasAdd/BiasAddGrad:output:0:gradient_tape/sequential/dense_2/MatMul/MatMul_1:product:0=gradient_tape/sequential/dense_2/BiasAdd/BiasAddGrad:output:08gradient_tape/sequential/dense_1/MatMul/MatMul:product:0=gradient_tape/sequential/dense_1/BiasAdd/BiasAddGrad:output:0:gradient_tape/sequential/dense_2/MatMul/MatMul_1:product:0=gradient_tape/sequential/dense_2/BiasAdd/BiasAddGrad:output:0*
T

2*)
_gradient_op_typeCustomGradient-416*\
_output_shapesJ
H:
��:�:	�
:
:
��:�:	�
:
�
SGD/StatefulPartitionedCallStatefulPartitionedCallIdentityN:output:01sequential_dense_1_matmul_readvariableop_resourcesgd_430sgd_432)^sequential/dense_1/MatMul/ReadVariableOp*
Tin
2*

Tout
 *
_XlaMustCompile(*
_collective_manager_ids
 *
_output_shapes
 *#
_read_only_resource_inputs
*0
config_proto 

CPU

GPU2*0J 8� *)
f$R"
 __inference__update_step_xla_247�
SGD/StatefulPartitionedCall_1StatefulPartitionedCallIdentityN:output:12sequential_dense_1_biasadd_readvariableop_resourcesgd_430sgd_436*^sequential/dense_1/BiasAdd/ReadVariableOp*
Tin
2*

Tout
 *
_XlaMustCompile(*
_collective_manager_ids
 *
_output_shapes
 *#
_read_only_resource_inputs
*0
config_proto 

CPU

GPU2*0J 8� *)
f$R"
 __inference__update_step_xla_266�
SGD/StatefulPartitionedCall_2StatefulPartitionedCallIdentityN:output:21sequential_dense_2_matmul_readvariableop_resourcesgd_430sgd_440)^sequential/dense_2/MatMul/ReadVariableOp*
Tin
2*

Tout
 *
_XlaMustCompile(*
_collective_manager_ids
 *
_output_shapes
 *#
_read_only_resource_inputs
*0
config_proto 

CPU

GPU2*0J 8� *)
f$R"
 __inference__update_step_xla_284�
SGD/StatefulPartitionedCall_3StatefulPartitionedCallIdentityN:output:32sequential_dense_2_biasadd_readvariableop_resourcesgd_430sgd_444*^sequential/dense_2/BiasAdd/ReadVariableOp*
Tin
2*

Tout
 *
_XlaMustCompile(*
_collective_manager_ids
 *
_output_shapes
 *#
_read_only_resource_inputs
*0
config_proto 

CPU

GPU2*0J 8� *)
f$R"
 __inference__update_step_xla_302K
	SGD/ConstConst*
_output_shapes
: *
dtype0	*
value	B	 R�
SGD/AssignAddVariableOpAssignAddVariableOp sgd_assignaddvariableop_resourceSGD/Const:output:0*
_output_shapes
 *
dtype0	p

Identity_4Identity0categorical_crossentropy/weighted_loss/value:z:0^NoOp*
T0*
_output_shapes
: �
NoOpNoOp^SGD/AssignAddVariableOp^SGD/StatefulPartitionedCall^SGD/StatefulPartitionedCall_1^SGD/StatefulPartitionedCall_2^SGD/StatefulPartitionedCall_3*^sequential/dense_1/BiasAdd/ReadVariableOp)^sequential/dense_1/MatMul/ReadVariableOp*^sequential/dense_2/BiasAdd/ReadVariableOp)^sequential/dense_2/MatMul/ReadVariableOp*
_output_shapes
 "!

identity_4Identity_4:output:0*(
_construction_contextkEagerRuntime*Q
_input_shapes@
>:���������:���������
: : : : : : : : : : 22
SGD/AssignAddVariableOpSGD/AssignAddVariableOp2>
SGD/StatefulPartitionedCall_1SGD/StatefulPartitionedCall_12>
SGD/StatefulPartitionedCall_2SGD/StatefulPartitionedCall_22>
SGD/StatefulPartitionedCall_3SGD/StatefulPartitionedCall_32:
SGD/StatefulPartitionedCallSGD/StatefulPartitionedCall2V
)sequential/dense_1/BiasAdd/ReadVariableOp)sequential/dense_1/BiasAdd/ReadVariableOp2T
(sequential/dense_1/MatMul/ReadVariableOp(sequential/dense_1/MatMul/ReadVariableOp2V
)sequential/dense_2/BiasAdd/ReadVariableOp)sequential/dense_2/BiasAdd/ReadVariableOp2T
(sequential/dense_2/MatMul/ReadVariableOp(sequential/dense_2/MatMul/ReadVariableOp:($
"
_user_specified_name
resource:#


_user_specified_name444:#	

_user_specified_name440:#

_user_specified_name436:#

_user_specified_name432:#

_user_specified_name430:($
"
_user_specified_name
resource:($
"
_user_specified_name
resource:($
"
_user_specified_name
resource:($
"
_user_specified_name
resource:JF
'
_output_shapes
:���������


_user_specified_namey:N J
+
_output_shapes
:���������

_user_specified_namex
�
�
'__inference_dense_2_layer_call_fn_91122

inputs
unknown:	�

	unknown_0:

identity��StatefulPartitionedCall�
StatefulPartitionedCallStatefulPartitionedCallinputsunknown	unknown_0*
Tin
2*
Tout
2*
_collective_manager_ids
 *'
_output_shapes
:���������
*$
_read_only_resource_inputs
*0
config_proto 

CPU

GPU2*0J 8� *K
fFRD
B__inference_dense_2_layer_call_and_return_conditional_losses_90989o
IdentityIdentity StatefulPartitionedCall:output:0^NoOp*
T0*'
_output_shapes
:���������
<
NoOpNoOp^StatefulPartitionedCall*
_output_shapes
 "
identityIdentity:output:0*(
_construction_contextkEagerRuntime*+
_input_shapes
:����������: : 22
StatefulPartitionedCallStatefulPartitionedCall:%!

_user_specified_name91118:%!

_user_specified_name91116:P L
(
_output_shapes
:����������
 
_user_specified_nameinputs
�
�
#__inference_signature_wrapper_90663
checkpoint_path
unknown:
��
	unknown_0:	�
	unknown_1:	�

	unknown_2:

identity��StatefulPartitionedCall�
StatefulPartitionedCallStatefulPartitionedCallcheckpoint_pathunknown	unknown_0	unknown_1	unknown_2*
Tin	
2*
Tout
2*
_collective_manager_ids
 *
_output_shapes
: *&
_read_only_resource_inputs
*0
config_proto 

CPU

GPU2*0J 8� *
fR
__inference_save_90564^
IdentityIdentity StatefulPartitionedCall:output:0^NoOp*
T0*
_output_shapes
: <
NoOpNoOp^StatefulPartitionedCall*
_output_shapes
 "
identityIdentity:output:0*(
_construction_contextkEagerRuntime*
_input_shapes

: : : : : 22
StatefulPartitionedCallStatefulPartitionedCall:%!

_user_specified_name90659:%!

_user_specified_name90657:%!

_user_specified_name90655:%!

_user_specified_name90653:G C

_output_shapes
: 
)
_user_specified_namecheckpoint_path
�3
�
!__inference__traced_restore_91318
file_prefix3
assignvariableop_dense_1_kernel:
��.
assignvariableop_1_dense_1_bias:	�4
!assignvariableop_2_dense_2_kernel:	�
-
assignvariableop_3_dense_2_bias:
&
assignvariableop_4_iteration:	 *
 assignvariableop_5_learning_rate: ;
'assignvariableop_6_sgd_m_dense_1_kernel:
��4
%assignvariableop_7_sgd_m_dense_1_bias:	�:
'assignvariableop_8_sgd_m_dense_2_kernel:	�
3
%assignvariableop_9_sgd_m_dense_2_bias:

identity_11��AssignVariableOp�AssignVariableOp_1�AssignVariableOp_2�AssignVariableOp_3�AssignVariableOp_4�AssignVariableOp_5�AssignVariableOp_6�AssignVariableOp_7�AssignVariableOp_8�AssignVariableOp_9�
RestoreV2/tensor_namesConst"/device:CPU:0*
_output_shapes
:*
dtype0*�
value�B�B<model/layer_with_weights-0/kernel/.ATTRIBUTES/VARIABLE_VALUEB:model/layer_with_weights-0/bias/.ATTRIBUTES/VARIABLE_VALUEB<model/layer_with_weights-1/kernel/.ATTRIBUTES/VARIABLE_VALUEB:model/layer_with_weights-1/bias/.ATTRIBUTES/VARIABLE_VALUEB6model/optimizer/_iterations/.ATTRIBUTES/VARIABLE_VALUEB9model/optimizer/_learning_rate/.ATTRIBUTES/VARIABLE_VALUEB7model/optimizer/_variables/1/.ATTRIBUTES/VARIABLE_VALUEB7model/optimizer/_variables/2/.ATTRIBUTES/VARIABLE_VALUEB7model/optimizer/_variables/3/.ATTRIBUTES/VARIABLE_VALUEB7model/optimizer/_variables/4/.ATTRIBUTES/VARIABLE_VALUEB_CHECKPOINTABLE_OBJECT_GRAPH�
RestoreV2/shape_and_slicesConst"/device:CPU:0*
_output_shapes
:*
dtype0*)
value BB B B B B B B B B B B �
	RestoreV2	RestoreV2file_prefixRestoreV2/tensor_names:output:0#RestoreV2/shape_and_slices:output:0"/device:CPU:0*@
_output_shapes.
,:::::::::::*
dtypes
2	[
IdentityIdentityRestoreV2:tensors:0"/device:CPU:0*
T0*
_output_shapes
:�
AssignVariableOpAssignVariableOpassignvariableop_dense_1_kernelIdentity:output:0"/device:CPU:0*&
 _has_manual_control_dependencies(*
_output_shapes
 *
dtype0]

Identity_1IdentityRestoreV2:tensors:1"/device:CPU:0*
T0*
_output_shapes
:�
AssignVariableOp_1AssignVariableOpassignvariableop_1_dense_1_biasIdentity_1:output:0"/device:CPU:0*&
 _has_manual_control_dependencies(*
_output_shapes
 *
dtype0]

Identity_2IdentityRestoreV2:tensors:2"/device:CPU:0*
T0*
_output_shapes
:�
AssignVariableOp_2AssignVariableOp!assignvariableop_2_dense_2_kernelIdentity_2:output:0"/device:CPU:0*&
 _has_manual_control_dependencies(*
_output_shapes
 *
dtype0]

Identity_3IdentityRestoreV2:tensors:3"/device:CPU:0*
T0*
_output_shapes
:�
AssignVariableOp_3AssignVariableOpassignvariableop_3_dense_2_biasIdentity_3:output:0"/device:CPU:0*&
 _has_manual_control_dependencies(*
_output_shapes
 *
dtype0]

Identity_4IdentityRestoreV2:tensors:4"/device:CPU:0*
T0	*
_output_shapes
:�
AssignVariableOp_4AssignVariableOpassignvariableop_4_iterationIdentity_4:output:0"/device:CPU:0*&
 _has_manual_control_dependencies(*
_output_shapes
 *
dtype0	]

Identity_5IdentityRestoreV2:tensors:5"/device:CPU:0*
T0*
_output_shapes
:�
AssignVariableOp_5AssignVariableOp assignvariableop_5_learning_rateIdentity_5:output:0"/device:CPU:0*&
 _has_manual_control_dependencies(*
_output_shapes
 *
dtype0]

Identity_6IdentityRestoreV2:tensors:6"/device:CPU:0*
T0*
_output_shapes
:�
AssignVariableOp_6AssignVariableOp'assignvariableop_6_sgd_m_dense_1_kernelIdentity_6:output:0"/device:CPU:0*&
 _has_manual_control_dependencies(*
_output_shapes
 *
dtype0]

Identity_7IdentityRestoreV2:tensors:7"/device:CPU:0*
T0*
_output_shapes
:�
AssignVariableOp_7AssignVariableOp%assignvariableop_7_sgd_m_dense_1_biasIdentity_7:output:0"/device:CPU:0*&
 _has_manual_control_dependencies(*
_output_shapes
 *
dtype0]

Identity_8IdentityRestoreV2:tensors:8"/device:CPU:0*
T0*
_output_shapes
:�
AssignVariableOp_8AssignVariableOp'assignvariableop_8_sgd_m_dense_2_kernelIdentity_8:output:0"/device:CPU:0*&
 _has_manual_control_dependencies(*
_output_shapes
 *
dtype0]

Identity_9IdentityRestoreV2:tensors:9"/device:CPU:0*
T0*
_output_shapes
:�
AssignVariableOp_9AssignVariableOp%assignvariableop_9_sgd_m_dense_2_biasIdentity_9:output:0"/device:CPU:0*&
 _has_manual_control_dependencies(*
_output_shapes
 *
dtype0Y
NoOpNoOp"/device:CPU:0*&
 _has_manual_control_dependencies(*
_output_shapes
 �
Identity_10Identityfile_prefix^AssignVariableOp^AssignVariableOp_1^AssignVariableOp_2^AssignVariableOp_3^AssignVariableOp_4^AssignVariableOp_5^AssignVariableOp_6^AssignVariableOp_7^AssignVariableOp_8^AssignVariableOp_9^NoOp"/device:CPU:0*
T0*
_output_shapes
: W
Identity_11IdentityIdentity_10:output:0^NoOp_1*
T0*
_output_shapes
: �
NoOp_1NoOp^AssignVariableOp^AssignVariableOp_1^AssignVariableOp_2^AssignVariableOp_3^AssignVariableOp_4^AssignVariableOp_5^AssignVariableOp_6^AssignVariableOp_7^AssignVariableOp_8^AssignVariableOp_9*
_output_shapes
 "#
identity_11Identity_11:output:0*(
_construction_contextkEagerRuntime*)
_input_shapes
: : : : : : : : : : : 2(
AssignVariableOp_1AssignVariableOp_12(
AssignVariableOp_2AssignVariableOp_22(
AssignVariableOp_3AssignVariableOp_32(
AssignVariableOp_4AssignVariableOp_42(
AssignVariableOp_5AssignVariableOp_52(
AssignVariableOp_6AssignVariableOp_62(
AssignVariableOp_7AssignVariableOp_72(
AssignVariableOp_8AssignVariableOp_82(
AssignVariableOp_9AssignVariableOp_92$
AssignVariableOpAssignVariableOp:2
.
,
_user_specified_nameSGD/m/dense_2/bias:4	0
.
_user_specified_nameSGD/m/dense_2/kernel:2.
,
_user_specified_nameSGD/m/dense_1/bias:40
.
_user_specified_nameSGD/m/dense_1/kernel:-)
'
_user_specified_namelearning_rate:)%
#
_user_specified_name	iteration:,(
&
_user_specified_namedense_2/bias:.*
(
_user_specified_namedense_2/kernel:,(
&
_user_specified_namedense_1/bias:.*
(
_user_specified_namedense_1/kernel:C ?

_output_shapes
: 
%
_user_specified_namefile_prefix
�	
�
B__inference_dense_2_layer_call_and_return_conditional_losses_91132

inputs1
matmul_readvariableop_resource:	�
-
biasadd_readvariableop_resource:

identity��BiasAdd/ReadVariableOp�MatMul/ReadVariableOpu
MatMul/ReadVariableOpReadVariableOpmatmul_readvariableop_resource*
_output_shapes
:	�
*
dtype0i
MatMulMatMulinputsMatMul/ReadVariableOp:value:0*
T0*'
_output_shapes
:���������
r
BiasAdd/ReadVariableOpReadVariableOpbiasadd_readvariableop_resource*
_output_shapes
:
*
dtype0v
BiasAddBiasAddMatMul:product:0BiasAdd/ReadVariableOp:value:0*
T0*'
_output_shapes
:���������
_
IdentityIdentityBiasAdd:output:0^NoOp*
T0*'
_output_shapes
:���������
S
NoOpNoOp^BiasAdd/ReadVariableOp^MatMul/ReadVariableOp*
_output_shapes
 "
identityIdentity:output:0*(
_construction_contextkEagerRuntime*+
_input_shapes
:����������: : 20
BiasAdd/ReadVariableOpBiasAdd/ReadVariableOp2.
MatMul/ReadVariableOpMatMul/ReadVariableOp:($
"
_user_specified_name
resource:($
"
_user_specified_name
resource:P L
(
_output_shapes
:����������
 
_user_specified_nameinputs
�
�
#__inference_signature_wrapper_90633
x
y
unknown:
��
	unknown_0:	�
	unknown_1:	�

	unknown_2:

	unknown_3: 
	unknown_4:
��
	unknown_5:	�
	unknown_6:	�

	unknown_7:

	unknown_8:	 
identity��StatefulPartitionedCall�
StatefulPartitionedCallStatefulPartitionedCallxyunknown	unknown_0	unknown_1	unknown_2	unknown_3	unknown_4	unknown_5	unknown_6	unknown_7	unknown_8*
Tin
2*
Tout
2*
_collective_manager_ids
 *
_output_shapes
: *#
_read_only_resource_inputs
*0
config_proto 

CPU

GPU2*0J 8� *
fR
__inference_train_449^
IdentityIdentity StatefulPartitionedCall:output:0^NoOp*
T0*
_output_shapes
: <
NoOpNoOp^StatefulPartitionedCall*
_output_shapes
 "
identityIdentity:output:0*(
_construction_contextkEagerRuntime*Q
_input_shapes@
>:���������:���������
: : : : : : : : : : 22
StatefulPartitionedCallStatefulPartitionedCall:%!

_user_specified_name90629:%
!

_user_specified_name90627:%	!

_user_specified_name90625:%!

_user_specified_name90623:%!

_user_specified_name90621:%!

_user_specified_name90619:%!

_user_specified_name90617:%!

_user_specified_name90615:%!

_user_specified_name90613:%!

_user_specified_name90611:JF
'
_output_shapes
:���������


_user_specified_namey:N J
+
_output_shapes
:���������

_user_specified_namex
�
�
 __inference__wrapped_model_90954
flatten_inputE
1sequential_dense_1_matmul_readvariableop_resource:
��A
2sequential_dense_1_biasadd_readvariableop_resource:	�D
1sequential_dense_2_matmul_readvariableop_resource:	�
@
2sequential_dense_2_biasadd_readvariableop_resource:

identity��)sequential/dense_1/BiasAdd/ReadVariableOp�(sequential/dense_1/MatMul/ReadVariableOp�)sequential/dense_2/BiasAdd/ReadVariableOp�(sequential/dense_2/MatMul/ReadVariableOpi
sequential/flatten/ConstConst*
_output_shapes
:*
dtype0*
valueB"����  �
sequential/flatten/ReshapeReshapeflatten_input!sequential/flatten/Const:output:0*
T0*(
_output_shapes
:�����������
(sequential/dense_1/MatMul/ReadVariableOpReadVariableOp1sequential_dense_1_matmul_readvariableop_resource* 
_output_shapes
:
��*
dtype0�
sequential/dense_1/MatMulMatMul#sequential/flatten/Reshape:output:00sequential/dense_1/MatMul/ReadVariableOp:value:0*
T0*(
_output_shapes
:�����������
)sequential/dense_1/BiasAdd/ReadVariableOpReadVariableOp2sequential_dense_1_biasadd_readvariableop_resource*
_output_shapes	
:�*
dtype0�
sequential/dense_1/BiasAddBiasAdd#sequential/dense_1/MatMul:product:01sequential/dense_1/BiasAdd/ReadVariableOp:value:0*
T0*(
_output_shapes
:����������w
sequential/dense_1/ReluRelu#sequential/dense_1/BiasAdd:output:0*
T0*(
_output_shapes
:�����������
(sequential/dense_2/MatMul/ReadVariableOpReadVariableOp1sequential_dense_2_matmul_readvariableop_resource*
_output_shapes
:	�
*
dtype0�
sequential/dense_2/MatMulMatMul%sequential/dense_1/Relu:activations:00sequential/dense_2/MatMul/ReadVariableOp:value:0*
T0*'
_output_shapes
:���������
�
)sequential/dense_2/BiasAdd/ReadVariableOpReadVariableOp2sequential_dense_2_biasadd_readvariableop_resource*
_output_shapes
:
*
dtype0�
sequential/dense_2/BiasAddBiasAdd#sequential/dense_2/MatMul:product:01sequential/dense_2/BiasAdd/ReadVariableOp:value:0*
T0*'
_output_shapes
:���������
r
IdentityIdentity#sequential/dense_2/BiasAdd:output:0^NoOp*
T0*'
_output_shapes
:���������
�
NoOpNoOp*^sequential/dense_1/BiasAdd/ReadVariableOp)^sequential/dense_1/MatMul/ReadVariableOp*^sequential/dense_2/BiasAdd/ReadVariableOp)^sequential/dense_2/MatMul/ReadVariableOp*
_output_shapes
 "
identityIdentity:output:0*(
_construction_contextkEagerRuntime*2
_input_shapes!
:���������: : : : 2V
)sequential/dense_1/BiasAdd/ReadVariableOp)sequential/dense_1/BiasAdd/ReadVariableOp2T
(sequential/dense_1/MatMul/ReadVariableOp(sequential/dense_1/MatMul/ReadVariableOp2V
)sequential/dense_2/BiasAdd/ReadVariableOp)sequential/dense_2/BiasAdd/ReadVariableOp2T
(sequential/dense_2/MatMul/ReadVariableOp(sequential/dense_2/MatMul/ReadVariableOp:($
"
_user_specified_name
resource:($
"
_user_specified_name
resource:($
"
_user_specified_name
resource:($
"
_user_specified_name
resource:Z V
+
_output_shapes
:���������
'
_user_specified_nameflatten_input
�
�
"__inference__update_step_xla_90872
gradient
variable:
��%
mul_readvariableop_resource: +
readvariableop_resource:
����AssignAddVariableOp�AssignVariableOp�ReadVariableOp�ReadVariableOp_1�mul/ReadVariableOpK
Cast/xConst*
_output_shapes
: *
dtype0*
valueB
 *    ?
NegNeggradient*
T0* 
_output_shapes
:
��f
mul/ReadVariableOpReadVariableOpmul_readvariableop_resource*
_output_shapes
: *
dtype0Z
mulMulNeg:y:0mul/ReadVariableOp:value:0*
T0* 
_output_shapes
:
��h
ReadVariableOpReadVariableOpreadvariableop_resource* 
_output_shapes
:
��*
dtype0`
mul_1MulReadVariableOp:value:0Cast/x:output:0*
T0* 
_output_shapes
:
��K
addAddV2mul:z:0	mul_1:z:0*
T0* 
_output_shapes
:
���
AssignVariableOpAssignVariableOpreadvariableop_resourceadd:z:0^ReadVariableOp*
_output_shapes
 *
dtype0*
validate_shape(}
ReadVariableOp_1ReadVariableOpreadvariableop_resource^AssignVariableOp* 
_output_shapes
:
��*
dtype0q
AssignAddVariableOpAssignAddVariableOpvariableReadVariableOp_1:value:0*
_output_shapes
 *
dtype0*
_XlaMustCompile(*(
_construction_contextkEagerRuntime*%
_input_shapes
:
��: : : *
	_noinline(2*
AssignAddVariableOpAssignAddVariableOp2$
AssignVariableOpAssignVariableOp2$
ReadVariableOp_1ReadVariableOp_12 
ReadVariableOpReadVariableOp2(
mul/ReadVariableOpmul/ReadVariableOp:($
"
_user_specified_name
resource:($
"
_user_specified_name
resource:($
"
_user_specified_name
variable:J F
 
_output_shapes
:
��
"
_user_specified_name
gradient
�
�
 __inference__update_step_xla_284
gradient
variable:	�
%
mul_readvariableop_resource: *
readvariableop_resource:	�
��AssignAddVariableOp�AssignVariableOp�ReadVariableOp�ReadVariableOp_1�mul/ReadVariableOpK
Cast/xConst*
_output_shapes
: *
dtype0*
valueB
 *    >
NegNeggradient*
T0*
_output_shapes
:	�
f
mul/ReadVariableOpReadVariableOpmul_readvariableop_resource*
_output_shapes
: *
dtype0Y
mulMulNeg:y:0mul/ReadVariableOp:value:0*
T0*
_output_shapes
:	�
g
ReadVariableOpReadVariableOpreadvariableop_resource*
_output_shapes
:	�
*
dtype0_
mul_1MulReadVariableOp:value:0Cast/x:output:0*
T0*
_output_shapes
:	�
J
addAddV2mul:z:0	mul_1:z:0*
T0*
_output_shapes
:	�
�
AssignVariableOpAssignVariableOpreadvariableop_resourceadd:z:0^ReadVariableOp*
_output_shapes
 *
dtype0*
validate_shape(|
ReadVariableOp_1ReadVariableOpreadvariableop_resource^AssignVariableOp*
_output_shapes
:	�
*
dtype0q
AssignAddVariableOpAssignAddVariableOpvariableReadVariableOp_1:value:0*
_output_shapes
 *
dtype0*
_XlaMustCompile(*(
_construction_contextkEagerRuntime*$
_input_shapes
:	�
: : : *
	_noinline(2*
AssignAddVariableOpAssignAddVariableOp2$
AssignVariableOpAssignVariableOp2$
ReadVariableOp_1ReadVariableOp_12 
ReadVariableOpReadVariableOp2(
mul/ReadVariableOpmul/ReadVariableOp:($
"
_user_specified_name
resource:($
"
_user_specified_name
resource:($
"
_user_specified_name
variable:I E

_output_shapes
:	�

"
_user_specified_name
gradient
�
^
B__inference_flatten_layer_call_and_return_conditional_losses_91093

inputs
identityV
ConstConst*
_output_shapes
:*
dtype0*
valueB"����  ]
ReshapeReshapeinputsConst:output:0*
T0*(
_output_shapes
:����������Y
IdentityIdentityReshape:output:0*
T0*(
_output_shapes
:����������"
identityIdentity:output:0*(
_construction_contextkEagerRuntime**
_input_shapes
:���������:S O
+
_output_shapes
:���������
 
_user_specified_nameinputs
�
�
 __inference__update_step_xla_302
gradient
variable:
%
mul_readvariableop_resource: %
readvariableop_resource:
��AssignAddVariableOp�AssignVariableOp�ReadVariableOp�ReadVariableOp_1�mul/ReadVariableOpK
Cast/xConst*
_output_shapes
: *
dtype0*
valueB
 *    9
NegNeggradient*
T0*
_output_shapes
:
f
mul/ReadVariableOpReadVariableOpmul_readvariableop_resource*
_output_shapes
: *
dtype0T
mulMulNeg:y:0mul/ReadVariableOp:value:0*
T0*
_output_shapes
:
b
ReadVariableOpReadVariableOpreadvariableop_resource*
_output_shapes
:
*
dtype0Z
mul_1MulReadVariableOp:value:0Cast/x:output:0*
T0*
_output_shapes
:
E
addAddV2mul:z:0	mul_1:z:0*
T0*
_output_shapes
:
�
AssignVariableOpAssignVariableOpreadvariableop_resourceadd:z:0^ReadVariableOp*
_output_shapes
 *
dtype0*
validate_shape(w
ReadVariableOp_1ReadVariableOpreadvariableop_resource^AssignVariableOp*
_output_shapes
:
*
dtype0q
AssignAddVariableOpAssignAddVariableOpvariableReadVariableOp_1:value:0*
_output_shapes
 *
dtype0*
_XlaMustCompile(*(
_construction_contextkEagerRuntime*
_input_shapes
:
: : : *
	_noinline(2*
AssignAddVariableOpAssignAddVariableOp2$
AssignVariableOpAssignVariableOp2$
ReadVariableOp_1ReadVariableOp_12 
ReadVariableOpReadVariableOp2(
mul/ReadVariableOpmul/ReadVariableOp:($
"
_user_specified_name
resource:($
"
_user_specified_name
resource:($
"
_user_specified_name
variable:D @

_output_shapes
:

"
_user_specified_name
gradient
�
�
#__inference_signature_wrapper_90683
checkpoint_path
unknown:
��
	unknown_0:	�
	unknown_1:	�

	unknown_2:

identity

identity_1

identity_2

identity_3��StatefulPartitionedCall�
StatefulPartitionedCallStatefulPartitionedCallcheckpoint_pathunknown	unknown_0	unknown_1	unknown_2*
Tin	
2*
Tout
2*
_collective_manager_ids
 *$
_output_shapes
::::* 
_read_only_resource_inputs
 *0
config_proto 

CPU

GPU2*0J 8� *"
fR
__inference_restore_90606`
IdentityIdentity StatefulPartitionedCall:output:0^NoOp*
T0*
_output_shapes
:b

Identity_1Identity StatefulPartitionedCall:output:1^NoOp*
T0*
_output_shapes
:b

Identity_2Identity StatefulPartitionedCall:output:2^NoOp*
T0*
_output_shapes
:b

Identity_3Identity StatefulPartitionedCall:output:3^NoOp*
T0*
_output_shapes
:<
NoOpNoOp^StatefulPartitionedCall*
_output_shapes
 "!

identity_1Identity_1:output:0"!

identity_2Identity_2:output:0"!

identity_3Identity_3:output:0"
identityIdentity:output:0*(
_construction_contextkEagerRuntime*
_input_shapes

: : : : : 22
StatefulPartitionedCallStatefulPartitionedCall:%!

_user_specified_name90673:%!

_user_specified_name90671:%!

_user_specified_name90669:%!

_user_specified_name90667:G C

_output_shapes
: 
)
_user_specified_namecheckpoint_path
�
�
E__inference_sequential_layer_call_and_return_conditional_losses_90996
flatten_input!
dense_1_90975:
��
dense_1_90977:	� 
dense_2_90990:	�

dense_2_90992:

identity��dense_1/StatefulPartitionedCall�dense_2/StatefulPartitionedCall�
flatten/PartitionedCallPartitionedCallflatten_input*
Tin
2*
Tout
2*
_collective_manager_ids
 *(
_output_shapes
:����������* 
_read_only_resource_inputs
 *0
config_proto 

CPU

GPU2*0J 8� *K
fFRD
B__inference_flatten_layer_call_and_return_conditional_losses_90962�
dense_1/StatefulPartitionedCallStatefulPartitionedCall flatten/PartitionedCall:output:0dense_1_90975dense_1_90977*
Tin
2*
Tout
2*
_collective_manager_ids
 *(
_output_shapes
:����������*$
_read_only_resource_inputs
*0
config_proto 

CPU

GPU2*0J 8� *K
fFRD
B__inference_dense_1_layer_call_and_return_conditional_losses_90974�
dense_2/StatefulPartitionedCallStatefulPartitionedCall(dense_1/StatefulPartitionedCall:output:0dense_2_90990dense_2_90992*
Tin
2*
Tout
2*
_collective_manager_ids
 *'
_output_shapes
:���������
*$
_read_only_resource_inputs
*0
config_proto 

CPU

GPU2*0J 8� *K
fFRD
B__inference_dense_2_layer_call_and_return_conditional_losses_90989w
IdentityIdentity(dense_2/StatefulPartitionedCall:output:0^NoOp*
T0*'
_output_shapes
:���������
f
NoOpNoOp ^dense_1/StatefulPartitionedCall ^dense_2/StatefulPartitionedCall*
_output_shapes
 "
identityIdentity:output:0*(
_construction_contextkEagerRuntime*2
_input_shapes!
:���������: : : : 2B
dense_1/StatefulPartitionedCalldense_1/StatefulPartitionedCall2B
dense_2/StatefulPartitionedCalldense_2/StatefulPartitionedCall:%!

_user_specified_name90992:%!

_user_specified_name90990:%!

_user_specified_name90977:%!

_user_specified_name90975:Z V
+
_output_shapes
:���������
'
_user_specified_nameflatten_input
�

�
#__inference_signature_wrapper_90649
x
unknown:
��
	unknown_0:	�
	unknown_1:	�

	unknown_2:

identity

identity_1��StatefulPartitionedCall�
StatefulPartitionedCallStatefulPartitionedCallxunknown	unknown_0	unknown_1	unknown_2*
Tin	
2*
Tout
2*
_collective_manager_ids
 *:
_output_shapes(
&:���������
:���������
*&
_read_only_resource_inputs
*0
config_proto 

CPU

GPU2*0J 8� * 
fR
__inference_infer_90587o
IdentityIdentity StatefulPartitionedCall:output:0^NoOp*
T0*'
_output_shapes
:���������
q

Identity_1Identity StatefulPartitionedCall:output:1^NoOp*
T0*'
_output_shapes
:���������
<
NoOpNoOp^StatefulPartitionedCall*
_output_shapes
 "!

identity_1Identity_1:output:0"
identityIdentity:output:0*(
_construction_contextkEagerRuntime*2
_input_shapes!
:���������: : : : 22
StatefulPartitionedCallStatefulPartitionedCall:%!

_user_specified_name90643:%!

_user_specified_name90641:%!

_user_specified_name90639:%!

_user_specified_name90637:N J
+
_output_shapes
:���������

_user_specified_namex
�
C
'__inference_flatten_layer_call_fn_91087

inputs
identity�
PartitionedCallPartitionedCallinputs*
Tin
2*
Tout
2*
_collective_manager_ids
 *(
_output_shapes
:����������* 
_read_only_resource_inputs
 *0
config_proto 

CPU

GPU2*0J 8� *K
fFRD
B__inference_flatten_layer_call_and_return_conditional_losses_90962a
IdentityIdentityPartitionedCall:output:0*
T0*(
_output_shapes
:����������"
identityIdentity:output:0*(
_construction_contextkEagerRuntime**
_input_shapes
:���������:S O
+
_output_shapes
:���������
 
_user_specified_nameinputs
�X
�	
__inference__traced_save_91279
file_prefix9
%read_disablecopyonread_dense_1_kernel:
��4
%read_1_disablecopyonread_dense_1_bias:	�:
'read_2_disablecopyonread_dense_2_kernel:	�
3
%read_3_disablecopyonread_dense_2_bias:
,
"read_4_disablecopyonread_iteration:	 0
&read_5_disablecopyonread_learning_rate: A
-read_6_disablecopyonread_sgd_m_dense_1_kernel:
��:
+read_7_disablecopyonread_sgd_m_dense_1_bias:	�@
-read_8_disablecopyonread_sgd_m_dense_2_kernel:	�
9
+read_9_disablecopyonread_sgd_m_dense_2_bias:

savev2_const
identity_21��MergeV2Checkpoints�Read/DisableCopyOnRead�Read/ReadVariableOp�Read_1/DisableCopyOnRead�Read_1/ReadVariableOp�Read_2/DisableCopyOnRead�Read_2/ReadVariableOp�Read_3/DisableCopyOnRead�Read_3/ReadVariableOp�Read_4/DisableCopyOnRead�Read_4/ReadVariableOp�Read_5/DisableCopyOnRead�Read_5/ReadVariableOp�Read_6/DisableCopyOnRead�Read_6/ReadVariableOp�Read_7/DisableCopyOnRead�Read_7/ReadVariableOp�Read_8/DisableCopyOnRead�Read_8/ReadVariableOp�Read_9/DisableCopyOnRead�Read_9/ReadVariableOpw
StaticRegexFullMatchStaticRegexFullMatchfile_prefix"/device:CPU:**
_output_shapes
: *
pattern
^s3://.*Z
ConstConst"/device:CPU:**
_output_shapes
: *
dtype0*
valueB B.parta
Const_1Const"/device:CPU:**
_output_shapes
: *
dtype0*
valueB B
_temp/part�
SelectSelectStaticRegexFullMatch:output:0Const:output:0Const_1:output:0"/device:CPU:**
T0*
_output_shapes
: f

StringJoin
StringJoinfile_prefixSelect:output:0"/device:CPU:**
N*
_output_shapes
: L

num_shardsConst*
_output_shapes
: *
dtype0*
value	B :f
ShardedFilename/shardConst"/device:CPU:0*
_output_shapes
: *
dtype0*
value	B : �
ShardedFilenameShardedFilenameStringJoin:output:0ShardedFilename/shard:output:0num_shards:output:0"/device:CPU:0*
_output_shapes
: w
Read/DisableCopyOnReadDisableCopyOnRead%read_disablecopyonread_dense_1_kernel"/device:CPU:0*
_output_shapes
 �
Read/ReadVariableOpReadVariableOp%read_disablecopyonread_dense_1_kernel^Read/DisableCopyOnRead"/device:CPU:0* 
_output_shapes
:
��*
dtype0k
IdentityIdentityRead/ReadVariableOp:value:0"/device:CPU:0*
T0* 
_output_shapes
:
��c

Identity_1IdentityIdentity:output:0"/device:CPU:0*
T0* 
_output_shapes
:
��y
Read_1/DisableCopyOnReadDisableCopyOnRead%read_1_disablecopyonread_dense_1_bias"/device:CPU:0*
_output_shapes
 �
Read_1/ReadVariableOpReadVariableOp%read_1_disablecopyonread_dense_1_bias^Read_1/DisableCopyOnRead"/device:CPU:0*
_output_shapes	
:�*
dtype0j

Identity_2IdentityRead_1/ReadVariableOp:value:0"/device:CPU:0*
T0*
_output_shapes	
:�`

Identity_3IdentityIdentity_2:output:0"/device:CPU:0*
T0*
_output_shapes	
:�{
Read_2/DisableCopyOnReadDisableCopyOnRead'read_2_disablecopyonread_dense_2_kernel"/device:CPU:0*
_output_shapes
 �
Read_2/ReadVariableOpReadVariableOp'read_2_disablecopyonread_dense_2_kernel^Read_2/DisableCopyOnRead"/device:CPU:0*
_output_shapes
:	�
*
dtype0n

Identity_4IdentityRead_2/ReadVariableOp:value:0"/device:CPU:0*
T0*
_output_shapes
:	�
d

Identity_5IdentityIdentity_4:output:0"/device:CPU:0*
T0*
_output_shapes
:	�
y
Read_3/DisableCopyOnReadDisableCopyOnRead%read_3_disablecopyonread_dense_2_bias"/device:CPU:0*
_output_shapes
 �
Read_3/ReadVariableOpReadVariableOp%read_3_disablecopyonread_dense_2_bias^Read_3/DisableCopyOnRead"/device:CPU:0*
_output_shapes
:
*
dtype0i

Identity_6IdentityRead_3/ReadVariableOp:value:0"/device:CPU:0*
T0*
_output_shapes
:
_

Identity_7IdentityIdentity_6:output:0"/device:CPU:0*
T0*
_output_shapes
:
v
Read_4/DisableCopyOnReadDisableCopyOnRead"read_4_disablecopyonread_iteration"/device:CPU:0*
_output_shapes
 �
Read_4/ReadVariableOpReadVariableOp"read_4_disablecopyonread_iteration^Read_4/DisableCopyOnRead"/device:CPU:0*
_output_shapes
: *
dtype0	e

Identity_8IdentityRead_4/ReadVariableOp:value:0"/device:CPU:0*
T0	*
_output_shapes
: [

Identity_9IdentityIdentity_8:output:0"/device:CPU:0*
T0	*
_output_shapes
: z
Read_5/DisableCopyOnReadDisableCopyOnRead&read_5_disablecopyonread_learning_rate"/device:CPU:0*
_output_shapes
 �
Read_5/ReadVariableOpReadVariableOp&read_5_disablecopyonread_learning_rate^Read_5/DisableCopyOnRead"/device:CPU:0*
_output_shapes
: *
dtype0f
Identity_10IdentityRead_5/ReadVariableOp:value:0"/device:CPU:0*
T0*
_output_shapes
: ]
Identity_11IdentityIdentity_10:output:0"/device:CPU:0*
T0*
_output_shapes
: �
Read_6/DisableCopyOnReadDisableCopyOnRead-read_6_disablecopyonread_sgd_m_dense_1_kernel"/device:CPU:0*
_output_shapes
 �
Read_6/ReadVariableOpReadVariableOp-read_6_disablecopyonread_sgd_m_dense_1_kernel^Read_6/DisableCopyOnRead"/device:CPU:0* 
_output_shapes
:
��*
dtype0p
Identity_12IdentityRead_6/ReadVariableOp:value:0"/device:CPU:0*
T0* 
_output_shapes
:
��g
Identity_13IdentityIdentity_12:output:0"/device:CPU:0*
T0* 
_output_shapes
:
��
Read_7/DisableCopyOnReadDisableCopyOnRead+read_7_disablecopyonread_sgd_m_dense_1_bias"/device:CPU:0*
_output_shapes
 �
Read_7/ReadVariableOpReadVariableOp+read_7_disablecopyonread_sgd_m_dense_1_bias^Read_7/DisableCopyOnRead"/device:CPU:0*
_output_shapes	
:�*
dtype0k
Identity_14IdentityRead_7/ReadVariableOp:value:0"/device:CPU:0*
T0*
_output_shapes	
:�b
Identity_15IdentityIdentity_14:output:0"/device:CPU:0*
T0*
_output_shapes	
:��
Read_8/DisableCopyOnReadDisableCopyOnRead-read_8_disablecopyonread_sgd_m_dense_2_kernel"/device:CPU:0*
_output_shapes
 �
Read_8/ReadVariableOpReadVariableOp-read_8_disablecopyonread_sgd_m_dense_2_kernel^Read_8/DisableCopyOnRead"/device:CPU:0*
_output_shapes
:	�
*
dtype0o
Identity_16IdentityRead_8/ReadVariableOp:value:0"/device:CPU:0*
T0*
_output_shapes
:	�
f
Identity_17IdentityIdentity_16:output:0"/device:CPU:0*
T0*
_output_shapes
:	�

Read_9/DisableCopyOnReadDisableCopyOnRead+read_9_disablecopyonread_sgd_m_dense_2_bias"/device:CPU:0*
_output_shapes
 �
Read_9/ReadVariableOpReadVariableOp+read_9_disablecopyonread_sgd_m_dense_2_bias^Read_9/DisableCopyOnRead"/device:CPU:0*
_output_shapes
:
*
dtype0j
Identity_18IdentityRead_9/ReadVariableOp:value:0"/device:CPU:0*
T0*
_output_shapes
:
a
Identity_19IdentityIdentity_18:output:0"/device:CPU:0*
T0*
_output_shapes
:
�
SaveV2/tensor_namesConst"/device:CPU:0*
_output_shapes
:*
dtype0*�
value�B�B<model/layer_with_weights-0/kernel/.ATTRIBUTES/VARIABLE_VALUEB:model/layer_with_weights-0/bias/.ATTRIBUTES/VARIABLE_VALUEB<model/layer_with_weights-1/kernel/.ATTRIBUTES/VARIABLE_VALUEB:model/layer_with_weights-1/bias/.ATTRIBUTES/VARIABLE_VALUEB6model/optimizer/_iterations/.ATTRIBUTES/VARIABLE_VALUEB9model/optimizer/_learning_rate/.ATTRIBUTES/VARIABLE_VALUEB7model/optimizer/_variables/1/.ATTRIBUTES/VARIABLE_VALUEB7model/optimizer/_variables/2/.ATTRIBUTES/VARIABLE_VALUEB7model/optimizer/_variables/3/.ATTRIBUTES/VARIABLE_VALUEB7model/optimizer/_variables/4/.ATTRIBUTES/VARIABLE_VALUEB_CHECKPOINTABLE_OBJECT_GRAPH�
SaveV2/shape_and_slicesConst"/device:CPU:0*
_output_shapes
:*
dtype0*)
value BB B B B B B B B B B B �
SaveV2SaveV2ShardedFilename:filename:0SaveV2/tensor_names:output:0 SaveV2/shape_and_slices:output:0Identity_1:output:0Identity_3:output:0Identity_5:output:0Identity_7:output:0Identity_9:output:0Identity_11:output:0Identity_13:output:0Identity_15:output:0Identity_17:output:0Identity_19:output:0savev2_const"/device:CPU:0*&
 _has_manual_control_dependencies(*
_output_shapes
 *
dtypes
2	�
&MergeV2Checkpoints/checkpoint_prefixesPackShardedFilename:filename:0^SaveV2"/device:CPU:0*
N*
T0*
_output_shapes
:�
MergeV2CheckpointsMergeV2Checkpoints/MergeV2Checkpoints/checkpoint_prefixes:output:0file_prefix"/device:CPU:0*&
 _has_manual_control_dependencies(*
_output_shapes
 i
Identity_20Identityfile_prefix^MergeV2Checkpoints"/device:CPU:0*
T0*
_output_shapes
: U
Identity_21IdentityIdentity_20:output:0^NoOp*
T0*
_output_shapes
: �
NoOpNoOp^MergeV2Checkpoints^Read/DisableCopyOnRead^Read/ReadVariableOp^Read_1/DisableCopyOnRead^Read_1/ReadVariableOp^Read_2/DisableCopyOnRead^Read_2/ReadVariableOp^Read_3/DisableCopyOnRead^Read_3/ReadVariableOp^Read_4/DisableCopyOnRead^Read_4/ReadVariableOp^Read_5/DisableCopyOnRead^Read_5/ReadVariableOp^Read_6/DisableCopyOnRead^Read_6/ReadVariableOp^Read_7/DisableCopyOnRead^Read_7/ReadVariableOp^Read_8/DisableCopyOnRead^Read_8/ReadVariableOp^Read_9/DisableCopyOnRead^Read_9/ReadVariableOp*
_output_shapes
 "#
identity_21Identity_21:output:0*(
_construction_contextkEagerRuntime*+
_input_shapes
: : : : : : : : : : : : 2(
MergeV2CheckpointsMergeV2Checkpoints20
Read/DisableCopyOnReadRead/DisableCopyOnRead2*
Read/ReadVariableOpRead/ReadVariableOp24
Read_1/DisableCopyOnReadRead_1/DisableCopyOnRead2.
Read_1/ReadVariableOpRead_1/ReadVariableOp24
Read_2/DisableCopyOnReadRead_2/DisableCopyOnRead2.
Read_2/ReadVariableOpRead_2/ReadVariableOp24
Read_3/DisableCopyOnReadRead_3/DisableCopyOnRead2.
Read_3/ReadVariableOpRead_3/ReadVariableOp24
Read_4/DisableCopyOnReadRead_4/DisableCopyOnRead2.
Read_4/ReadVariableOpRead_4/ReadVariableOp24
Read_5/DisableCopyOnReadRead_5/DisableCopyOnRead2.
Read_5/ReadVariableOpRead_5/ReadVariableOp24
Read_6/DisableCopyOnReadRead_6/DisableCopyOnRead2.
Read_6/ReadVariableOpRead_6/ReadVariableOp24
Read_7/DisableCopyOnReadRead_7/DisableCopyOnRead2.
Read_7/ReadVariableOpRead_7/ReadVariableOp24
Read_8/DisableCopyOnReadRead_8/DisableCopyOnRead2.
Read_8/ReadVariableOpRead_8/ReadVariableOp24
Read_9/DisableCopyOnReadRead_9/DisableCopyOnRead2.
Read_9/ReadVariableOpRead_9/ReadVariableOp:=9

_output_shapes
: 

_user_specified_nameConst:2
.
,
_user_specified_nameSGD/m/dense_2/bias:4	0
.
_user_specified_nameSGD/m/dense_2/kernel:2.
,
_user_specified_nameSGD/m/dense_1/bias:40
.
_user_specified_nameSGD/m/dense_1/kernel:-)
'
_user_specified_namelearning_rate:)%
#
_user_specified_name	iteration:,(
&
_user_specified_namedense_2/bias:.*
(
_user_specified_namedense_2/kernel:,(
&
_user_specified_namedense_1/bias:.*
(
_user_specified_namedense_1/kernel:C ?

_output_shapes
: 
%
_user_specified_namefile_prefix
�
�
 __inference__update_step_xla_247
gradient
variable:
��%
mul_readvariableop_resource: +
readvariableop_resource:
����AssignAddVariableOp�AssignVariableOp�ReadVariableOp�ReadVariableOp_1�mul/ReadVariableOpK
Cast/xConst*
_output_shapes
: *
dtype0*
valueB
 *    ?
NegNeggradient*
T0* 
_output_shapes
:
��f
mul/ReadVariableOpReadVariableOpmul_readvariableop_resource*
_output_shapes
: *
dtype0Z
mulMulNeg:y:0mul/ReadVariableOp:value:0*
T0* 
_output_shapes
:
��h
ReadVariableOpReadVariableOpreadvariableop_resource* 
_output_shapes
:
��*
dtype0`
mul_1MulReadVariableOp:value:0Cast/x:output:0*
T0* 
_output_shapes
:
��K
addAddV2mul:z:0	mul_1:z:0*
T0* 
_output_shapes
:
���
AssignVariableOpAssignVariableOpreadvariableop_resourceadd:z:0^ReadVariableOp*
_output_shapes
 *
dtype0*
validate_shape(}
ReadVariableOp_1ReadVariableOpreadvariableop_resource^AssignVariableOp* 
_output_shapes
:
��*
dtype0q
AssignAddVariableOpAssignAddVariableOpvariableReadVariableOp_1:value:0*
_output_shapes
 *
dtype0*
_XlaMustCompile(*(
_construction_contextkEagerRuntime*%
_input_shapes
:
��: : : *
	_noinline(2*
AssignAddVariableOpAssignAddVariableOp2$
AssignVariableOpAssignVariableOp2$
ReadVariableOp_1ReadVariableOp_12 
ReadVariableOpReadVariableOp2(
mul/ReadVariableOpmul/ReadVariableOp:($
"
_user_specified_name
resource:($
"
_user_specified_name
resource:($
"
_user_specified_name
variable:J F
 
_output_shapes
:
��
"
_user_specified_name
gradient
�	
�
B__inference_dense_2_layer_call_and_return_conditional_losses_90989

inputs1
matmul_readvariableop_resource:	�
-
biasadd_readvariableop_resource:

identity��BiasAdd/ReadVariableOp�MatMul/ReadVariableOpu
MatMul/ReadVariableOpReadVariableOpmatmul_readvariableop_resource*
_output_shapes
:	�
*
dtype0i
MatMulMatMulinputsMatMul/ReadVariableOp:value:0*
T0*'
_output_shapes
:���������
r
BiasAdd/ReadVariableOpReadVariableOpbiasadd_readvariableop_resource*
_output_shapes
:
*
dtype0v
BiasAddBiasAddMatMul:product:0BiasAdd/ReadVariableOp:value:0*
T0*'
_output_shapes
:���������
_
IdentityIdentityBiasAdd:output:0^NoOp*
T0*'
_output_shapes
:���������
S
NoOpNoOp^BiasAdd/ReadVariableOp^MatMul/ReadVariableOp*
_output_shapes
 "
identityIdentity:output:0*(
_construction_contextkEagerRuntime*+
_input_shapes
:����������: : 20
BiasAdd/ReadVariableOpBiasAdd/ReadVariableOp2.
MatMul/ReadVariableOpMatMul/ReadVariableOp:($
"
_user_specified_name
resource:($
"
_user_specified_name
resource:P L
(
_output_shapes
:����������
 
_user_specified_nameinputs
�
�
'__inference_dense_1_layer_call_fn_91102

inputs
unknown:
��
	unknown_0:	�
identity��StatefulPartitionedCall�
StatefulPartitionedCallStatefulPartitionedCallinputsunknown	unknown_0*
Tin
2*
Tout
2*
_collective_manager_ids
 *(
_output_shapes
:����������*$
_read_only_resource_inputs
*0
config_proto 

CPU

GPU2*0J 8� *K
fFRD
B__inference_dense_1_layer_call_and_return_conditional_losses_90974p
IdentityIdentity StatefulPartitionedCall:output:0^NoOp*
T0*(
_output_shapes
:����������<
NoOpNoOp^StatefulPartitionedCall*
_output_shapes
 "
identityIdentity:output:0*(
_construction_contextkEagerRuntime*+
_input_shapes
:����������: : 22
StatefulPartitionedCallStatefulPartitionedCall:%!

_user_specified_name91098:%!

_user_specified_name91096:P L
(
_output_shapes
:����������
 
_user_specified_nameinputs
�
J
"__inference__update_step_xla_91082
gradient
variable:
*
_XlaMustCompile(*(
_construction_contextkEagerRuntime*
_input_shapes

:
: *
	_noinline(:($
"
_user_specified_name
variable:D @

_output_shapes
:

"
_user_specified_name
gradient
�
P
"__inference__update_step_xla_91067
gradient
variable:
��*
_XlaMustCompile(*(
_construction_contextkEagerRuntime*!
_input_shapes
:
��: *
	_noinline(:($
"
_user_specified_name
variable:J F
 
_output_shapes
:
��
"
_user_specified_name
gradient
�
K
"__inference__update_step_xla_91072
gradient
variable:	�*
_XlaMustCompile(*(
_construction_contextkEagerRuntime*
_input_shapes
	:�: *
	_noinline(:($
"
_user_specified_name
variable:E A

_output_shapes	
:�
"
_user_specified_name
gradient
�	
�
*__inference_sequential_layer_call_fn_91037
flatten_input
unknown:
��
	unknown_0:	�
	unknown_1:	�

	unknown_2:

identity��StatefulPartitionedCall�
StatefulPartitionedCallStatefulPartitionedCallflatten_inputunknown	unknown_0	unknown_1	unknown_2*
Tin	
2*
Tout
2*
_collective_manager_ids
 *'
_output_shapes
:���������
*&
_read_only_resource_inputs
*0
config_proto 

CPU

GPU2*0J 8� *N
fIRG
E__inference_sequential_layer_call_and_return_conditional_losses_91011o
IdentityIdentity StatefulPartitionedCall:output:0^NoOp*
T0*'
_output_shapes
:���������
<
NoOpNoOp^StatefulPartitionedCall*
_output_shapes
 "
identityIdentity:output:0*(
_construction_contextkEagerRuntime*2
_input_shapes!
:���������: : : : 22
StatefulPartitionedCallStatefulPartitionedCall:%!

_user_specified_name91033:%!

_user_specified_name91031:%!

_user_specified_name91029:%!

_user_specified_name91027:Z V
+
_output_shapes
:���������
'
_user_specified_nameflatten_input
�
�
"__inference__update_step_xla_90909
gradient
variable:	�
%
mul_readvariableop_resource: *
readvariableop_resource:	�
��AssignAddVariableOp�AssignVariableOp�ReadVariableOp�ReadVariableOp_1�mul/ReadVariableOpK
Cast/xConst*
_output_shapes
: *
dtype0*
valueB
 *    >
NegNeggradient*
T0*
_output_shapes
:	�
f
mul/ReadVariableOpReadVariableOpmul_readvariableop_resource*
_output_shapes
: *
dtype0Y
mulMulNeg:y:0mul/ReadVariableOp:value:0*
T0*
_output_shapes
:	�
g
ReadVariableOpReadVariableOpreadvariableop_resource*
_output_shapes
:	�
*
dtype0_
mul_1MulReadVariableOp:value:0Cast/x:output:0*
T0*
_output_shapes
:	�
J
addAddV2mul:z:0	mul_1:z:0*
T0*
_output_shapes
:	�
�
AssignVariableOpAssignVariableOpreadvariableop_resourceadd:z:0^ReadVariableOp*
_output_shapes
 *
dtype0*
validate_shape(|
ReadVariableOp_1ReadVariableOpreadvariableop_resource^AssignVariableOp*
_output_shapes
:	�
*
dtype0q
AssignAddVariableOpAssignAddVariableOpvariableReadVariableOp_1:value:0*
_output_shapes
 *
dtype0*
_XlaMustCompile(*(
_construction_contextkEagerRuntime*$
_input_shapes
:	�
: : : *
	_noinline(2*
AssignAddVariableOpAssignAddVariableOp2$
AssignVariableOpAssignVariableOp2$
ReadVariableOp_1ReadVariableOp_12 
ReadVariableOpReadVariableOp2(
mul/ReadVariableOpmul/ReadVariableOp:($
"
_user_specified_name
resource:($
"
_user_specified_name
resource:($
"
_user_specified_name
variable:I E

_output_shapes
:	�

"
_user_specified_name
gradient
�

�
B__inference_dense_1_layer_call_and_return_conditional_losses_90974

inputs2
matmul_readvariableop_resource:
��.
biasadd_readvariableop_resource:	�
identity��BiasAdd/ReadVariableOp�MatMul/ReadVariableOpv
MatMul/ReadVariableOpReadVariableOpmatmul_readvariableop_resource* 
_output_shapes
:
��*
dtype0j
MatMulMatMulinputsMatMul/ReadVariableOp:value:0*
T0*(
_output_shapes
:����������s
BiasAdd/ReadVariableOpReadVariableOpbiasadd_readvariableop_resource*
_output_shapes	
:�*
dtype0w
BiasAddBiasAddMatMul:product:0BiasAdd/ReadVariableOp:value:0*
T0*(
_output_shapes
:����������Q
ReluReluBiasAdd:output:0*
T0*(
_output_shapes
:����������b
IdentityIdentityRelu:activations:0^NoOp*
T0*(
_output_shapes
:����������S
NoOpNoOp^BiasAdd/ReadVariableOp^MatMul/ReadVariableOp*
_output_shapes
 "
identityIdentity:output:0*(
_construction_contextkEagerRuntime*+
_input_shapes
:����������: : 20
BiasAdd/ReadVariableOpBiasAdd/ReadVariableOp2.
MatMul/ReadVariableOpMatMul/ReadVariableOp:($
"
_user_specified_name
resource:($
"
_user_specified_name
resource:P L
(
_output_shapes
:����������
 
_user_specified_nameinputs
�	
�
*__inference_sequential_layer_call_fn_91024
flatten_input
unknown:
��
	unknown_0:	�
	unknown_1:	�

	unknown_2:

identity��StatefulPartitionedCall�
StatefulPartitionedCallStatefulPartitionedCallflatten_inputunknown	unknown_0	unknown_1	unknown_2*
Tin	
2*
Tout
2*
_collective_manager_ids
 *'
_output_shapes
:���������
*&
_read_only_resource_inputs
*0
config_proto 

CPU

GPU2*0J 8� *N
fIRG
E__inference_sequential_layer_call_and_return_conditional_losses_90996o
IdentityIdentity StatefulPartitionedCall:output:0^NoOp*
T0*'
_output_shapes
:���������
<
NoOpNoOp^StatefulPartitionedCall*
_output_shapes
 "
identityIdentity:output:0*(
_construction_contextkEagerRuntime*2
_input_shapes!
:���������: : : : 22
StatefulPartitionedCallStatefulPartitionedCall:%!

_user_specified_name91020:%!

_user_specified_name91018:%!

_user_specified_name91016:%!

_user_specified_name91014:Z V
+
_output_shapes
:���������
'
_user_specified_nameflatten_input
�
�
__inference_save_90564
checkpoint_path0
read_readvariableop_resource:
��-
read_1_readvariableop_resource:	�1
read_2_readvariableop_resource:	�
,
read_3_readvariableop_resource:


identity_4��Read/ReadVariableOp�Read_1/ReadVariableOp�Read_2/ReadVariableOp�Read_3/ReadVariableOp�saver
Read/ReadVariableOpReadVariableOpread_readvariableop_resource* 
_output_shapes
:
��*
dtype0\
IdentityIdentityRead/ReadVariableOp:value:0*
T0* 
_output_shapes
:
��q
Read_1/ReadVariableOpReadVariableOpread_1_readvariableop_resource*
_output_shapes	
:�*
dtype0[

Identity_1IdentityRead_1/ReadVariableOp:value:0*
T0*
_output_shapes	
:�u
Read_2/ReadVariableOpReadVariableOpread_2_readvariableop_resource*
_output_shapes
:	�
*
dtype0_

Identity_2IdentityRead_2/ReadVariableOp:value:0*
T0*
_output_shapes
:	�
p
Read_3/ReadVariableOpReadVariableOpread_3_readvariableop_resource*
_output_shapes
:
*
dtype0Z

Identity_3IdentityRead_3/ReadVariableOp:value:0*
T0*
_output_shapes
:
�
save/tensor_namesConst*
_output_shapes
:*
dtype0*W
valueNBLBdense_1/kernel:0Bdense_1/bias:0Bdense_2/kernel:0Bdense_2/bias:0�
saveSavecheckpoint_pathsave/tensor_names:output:0Identity:output:0Identity_1:output:0Identity_2:output:0Identity_3:output:0*
T
2*
_output_shapes
 O

Identity_4Identitycheckpoint_path^NoOp*
T0*
_output_shapes
: �
NoOpNoOp^Read/ReadVariableOp^Read_1/ReadVariableOp^Read_2/ReadVariableOp^Read_3/ReadVariableOp^save*
_output_shapes
 "!

identity_4Identity_4:output:0*(
_construction_contextkEagerRuntime*
_input_shapes

: : : : : 2*
Read/ReadVariableOpRead/ReadVariableOp2.
Read_1/ReadVariableOpRead_1/ReadVariableOp2.
Read_2/ReadVariableOpRead_2/ReadVariableOp2.
Read_3/ReadVariableOpRead_3/ReadVariableOp2
savesave:($
"
_user_specified_name
resource:($
"
_user_specified_name
resource:($
"
_user_specified_name
resource:($
"
_user_specified_name
resource:G C

_output_shapes
: 
)
_user_specified_namecheckpoint_path
�
�
"__inference_internal_grad_fn_91220
result_grads_0
result_grads_1
result_grads_2
result_grads_3
result_grads_4
result_grads_5
result_grads_6
result_grads_7

identity_4

identity_5

identity_6

identity_7O
IdentityIdentityresult_grads_0*
T0* 
_output_shapes
:
��L

Identity_1Identityresult_grads_1*
T0*
_output_shapes	
:�P

Identity_2Identityresult_grads_2*
T0*
_output_shapes
:	�
K

Identity_3Identityresult_grads_3*
T0*
_output_shapes
:
�
	IdentityN	IdentityNresult_grads_0result_grads_1result_grads_2result_grads_3result_grads_0result_grads_1result_grads_2result_grads_3*
T

2*+
_gradient_op_typeCustomGradient-91203*\
_output_shapesJ
H:
��:�:	�
:
:
��:�:	�
:
U

Identity_4IdentityIdentityN:output:0*
T0* 
_output_shapes
:
��P

Identity_5IdentityIdentityN:output:1*
T0*
_output_shapes	
:�T

Identity_6IdentityIdentityN:output:2*
T0*
_output_shapes
:	�
O

Identity_7IdentityIdentityN:output:3*
T0*
_output_shapes
:
"!

identity_4Identity_4:output:0"!

identity_5Identity_5:output:0"!

identity_6Identity_6:output:0"!

identity_7Identity_7:output:0*(
_construction_contextkEagerRuntime*[
_input_shapesJ
H:
��:�:	�
:
:
��:�:	�
:
:JF

_output_shapes
:

(
_user_specified_nameresult_grads_7:OK

_output_shapes
:	�

(
_user_specified_nameresult_grads_6:KG

_output_shapes	
:�
(
_user_specified_nameresult_grads_5:PL
 
_output_shapes
:
��
(
_user_specified_nameresult_grads_4:JF

_output_shapes
:

(
_user_specified_nameresult_grads_3:OK

_output_shapes
:	�

(
_user_specified_nameresult_grads_2:KG

_output_shapes	
:�
(
_user_specified_nameresult_grads_1:P L
 
_output_shapes
:
��
(
_user_specified_nameresult_grads_0
�
�
 __inference__update_step_xla_266
gradient
variable:	�%
mul_readvariableop_resource: &
readvariableop_resource:	���AssignAddVariableOp�AssignVariableOp�ReadVariableOp�ReadVariableOp_1�mul/ReadVariableOpK
Cast/xConst*
_output_shapes
: *
dtype0*
valueB
 *    :
NegNeggradient*
T0*
_output_shapes	
:�f
mul/ReadVariableOpReadVariableOpmul_readvariableop_resource*
_output_shapes
: *
dtype0U
mulMulNeg:y:0mul/ReadVariableOp:value:0*
T0*
_output_shapes	
:�c
ReadVariableOpReadVariableOpreadvariableop_resource*
_output_shapes	
:�*
dtype0[
mul_1MulReadVariableOp:value:0Cast/x:output:0*
T0*
_output_shapes	
:�F
addAddV2mul:z:0	mul_1:z:0*
T0*
_output_shapes	
:��
AssignVariableOpAssignVariableOpreadvariableop_resourceadd:z:0^ReadVariableOp*
_output_shapes
 *
dtype0*
validate_shape(x
ReadVariableOp_1ReadVariableOpreadvariableop_resource^AssignVariableOp*
_output_shapes	
:�*
dtype0q
AssignAddVariableOpAssignAddVariableOpvariableReadVariableOp_1:value:0*
_output_shapes
 *
dtype0*
_XlaMustCompile(*(
_construction_contextkEagerRuntime* 
_input_shapes
:�: : : *
	_noinline(2*
AssignAddVariableOpAssignAddVariableOp2$
AssignVariableOpAssignVariableOp2$
ReadVariableOp_1ReadVariableOp_12 
ReadVariableOpReadVariableOp2(
mul/ReadVariableOpmul/ReadVariableOp:($
"
_user_specified_name
resource:($
"
_user_specified_name
resource:($
"
_user_specified_name
variable:E A

_output_shapes	
:�
"
_user_specified_name
gradient
�
�
"__inference_internal_grad_fn_91193
result_grads_0
result_grads_1
result_grads_2
result_grads_3
result_grads_4
result_grads_5
result_grads_6
result_grads_7

identity_4

identity_5

identity_6

identity_7O
IdentityIdentityresult_grads_0*
T0* 
_output_shapes
:
��L

Identity_1Identityresult_grads_1*
T0*
_output_shapes	
:�P

Identity_2Identityresult_grads_2*
T0*
_output_shapes
:	�
K

Identity_3Identityresult_grads_3*
T0*
_output_shapes
:
�
	IdentityN	IdentityNresult_grads_0result_grads_1result_grads_2result_grads_3result_grads_0result_grads_1result_grads_2result_grads_3*
T

2*+
_gradient_op_typeCustomGradient-91176*\
_output_shapesJ
H:
��:�:	�
:
:
��:�:	�
:
U

Identity_4IdentityIdentityN:output:0*
T0* 
_output_shapes
:
��P

Identity_5IdentityIdentityN:output:1*
T0*
_output_shapes	
:�T

Identity_6IdentityIdentityN:output:2*
T0*
_output_shapes
:	�
O

Identity_7IdentityIdentityN:output:3*
T0*
_output_shapes
:
"!

identity_4Identity_4:output:0"!

identity_5Identity_5:output:0"!

identity_6Identity_6:output:0"!

identity_7Identity_7:output:0*(
_construction_contextkEagerRuntime*[
_input_shapesJ
H:
��:�:	�
:
:
��:�:	�
:
:JF

_output_shapes
:

(
_user_specified_nameresult_grads_7:OK

_output_shapes
:	�

(
_user_specified_nameresult_grads_6:KG

_output_shapes	
:�
(
_user_specified_nameresult_grads_5:PL
 
_output_shapes
:
��
(
_user_specified_nameresult_grads_4:JF

_output_shapes
:

(
_user_specified_nameresult_grads_3:OK

_output_shapes
:	�

(
_user_specified_nameresult_grads_2:KG

_output_shapes	
:�
(
_user_specified_nameresult_grads_1:P L
 
_output_shapes
:
��
(
_user_specified_nameresult_grads_0
�
�
__inference_restore_90723
checkpoint_path-
assignvariableop_resource:
��*
assignvariableop_1_resource:	�.
assignvariableop_2_resource:	�
)
assignvariableop_3_resource:

identity

identity_1

identity_2

identity_3��AssignVariableOp�AssignVariableOp_1�AssignVariableOp_2�AssignVariableOp_3�restore�	restore_1�	restore_2�	restore_3d
restore/tensor_nameConst*
_output_shapes
: *
dtype0*!
valueB Bdense_1/kernel:0e
restoreRestorecheckpoint_pathrestore/tensor_name:output:0*
_output_shapes
:*
dt0�
AssignVariableOpAssignVariableOpassignvariableop_resourcerestore:tensor:0*
_output_shapes
 *
dtype0*
validate_shape(d
restore_1/tensor_nameConst*
_output_shapes
: *
dtype0*
valueB Bdense_1/bias:0s
	restore_1Restorecheckpoint_pathrestore_1/tensor_name:output:0^restore*
_output_shapes
:*
dt0�
AssignVariableOp_1AssignVariableOpassignvariableop_1_resourcerestore_1:tensor:0*
_output_shapes
 *
dtype0*
validate_shape(f
restore_2/tensor_nameConst*
_output_shapes
: *
dtype0*!
valueB Bdense_2/kernel:0u
	restore_2Restorecheckpoint_pathrestore_2/tensor_name:output:0
^restore_1*
_output_shapes
:*
dt0�
AssignVariableOp_2AssignVariableOpassignvariableop_2_resourcerestore_2:tensor:0*
_output_shapes
 *
dtype0*
validate_shape(d
restore_3/tensor_nameConst*
_output_shapes
: *
dtype0*
valueB Bdense_2/bias:0u
	restore_3Restorecheckpoint_pathrestore_3/tensor_name:output:0
^restore_2*
_output_shapes
:*
dt0�
AssignVariableOp_3AssignVariableOpassignvariableop_3_resourcerestore_3:tensor:0*
_output_shapes
 *
dtype0*
validate_shape(R
IdentityIdentityrestore_1:tensor:0^NoOp*
T0*
_output_shapes
:R

Identity_1Identityrestore:tensor:0^NoOp*
T0*
_output_shapes
:T

Identity_2Identityrestore_3:tensor:0^NoOp*
T0*
_output_shapes
:T

Identity_3Identityrestore_2:tensor:0^NoOp*
T0*
_output_shapes
:�
NoOpNoOp^AssignVariableOp^AssignVariableOp_1^AssignVariableOp_2^AssignVariableOp_3^restore
^restore_1
^restore_2
^restore_3*
_output_shapes
 "!

identity_1Identity_1:output:0"!

identity_2Identity_2:output:0"!

identity_3Identity_3:output:0"
identityIdentity:output:0*(
_construction_contextkEagerRuntime*
_input_shapes

: : : : : 2(
AssignVariableOp_1AssignVariableOp_12(
AssignVariableOp_2AssignVariableOp_22(
AssignVariableOp_3AssignVariableOp_32$
AssignVariableOpAssignVariableOp2
	restore_1	restore_12
	restore_2	restore_22
	restore_3	restore_32
restorerestore:($
"
_user_specified_name
resource:($
"
_user_specified_name
resource:($
"
_user_specified_name
resource:($
"
_user_specified_name
resource:G C

_output_shapes
: 
)
_user_specified_namecheckpoint_path:
"__inference_internal_grad_fn_91193CustomGradient-908468
"__inference_internal_grad_fn_91220CustomGradient-416"�L
saver_filename:0StatefulPartitionedCall_4:0StatefulPartitionedCall_58"
saved_model_main_op

NoOp*>
__saved_model_init_op%#
__saved_model_init_op

NoOp*�
infer�
)
x$
	infer_x:0���������:
logits0
StatefulPartitionedCall:0���������
:
output0
StatefulPartitionedCall:1���������
tensorflow/serving/predict*�
restore�
2
checkpoint_path
restore_checkpoint_path:0 5
dense_1/bias:0#
StatefulPartitionedCall_1:07
dense_1/kernel:0#
StatefulPartitionedCall_1:15
dense_2/bias:0#
StatefulPartitionedCall_1:27
dense_2/kernel:0#
StatefulPartitionedCall_1:3tensorflow/serving/predict*�
save�
/
checkpoint_path
save_checkpoint_path:0 4
checkpoint_path!
StatefulPartitionedCall_2:0 tensorflow/serving/predict*�
train�
)
x$
	train_x:0���������
%
y 
	train_y:0���������
)
loss!
StatefulPartitionedCall_3:0 tensorflow/serving/predict:��
f
	model
	infer
restore
save
	train

signatures"
_generic_user_object
�
layer-0
layer_with_weights-0
layer-1
	layer_with_weights-1
	layer-2

	variables
trainable_variables
regularization_losses
	keras_api
__call__
*&call_and_return_all_conditional_losses
_default_save_signature
	optimizer"
_tf_keras_sequential
�
trace_02�
__inference_infer_90704�
���
FullArgSpec
args�
jx
varargs
 
varkw
 
defaults
 

kwonlyargs� 
kwonlydefaults
 
annotations� *!�
����������ztrace_0
�
trace_02�
__inference_restore_90723�
���
FullArgSpec
args�
jcheckpoint_path
varargs
 
varkw
 
defaults
 

kwonlyargs� 
kwonlydefaults
 
annotations� *�	
� ztrace_0
�
trace_02�
__inference_save_90740�
���
FullArgSpec
args�
jcheckpoint_path
varargs
 
varkw
 
defaults
 

kwonlyargs� 
kwonlydefaults
 
annotations� *�	
� ztrace_0
�
trace_02�
__inference_train_90935�
���
FullArgSpec
args�

jx
jy
varargs
 
varkw
 
defaults
 

kwonlyargs� 
kwonlydefaults
 
annotations� *;�8
����������
����������
ztrace_0
D
	train
	infer
save
restore"
signature_map
�
	variables
trainable_variables
regularization_losses
	keras_api
__call__
*&call_and_return_all_conditional_losses"
_tf_keras_layer
�
 	variables
!trainable_variables
"regularization_losses
#	keras_api
$__call__
*%&call_and_return_all_conditional_losses

&kernel
'bias"
_tf_keras_layer
�
(	variables
)trainable_variables
*regularization_losses
+	keras_api
,__call__
*-&call_and_return_all_conditional_losses

.kernel
/bias"
_tf_keras_layer
<
&0
'1
.2
/3"
trackable_list_wrapper
<
&0
'1
.2
/3"
trackable_list_wrapper
 "
trackable_list_wrapper
�
0non_trainable_variables

1layers
2metrics
3layer_regularization_losses
4layer_metrics

	variables
trainable_variables
regularization_losses
__call__
_default_save_signature
*&call_and_return_all_conditional_losses
&"call_and_return_conditional_losses"
_generic_user_object
�
5trace_0
6trace_12�
*__inference_sequential_layer_call_fn_91024
*__inference_sequential_layer_call_fn_91037�
���
FullArgSpec)
args!�
jinputs

jtraining
jmask
varargs
 
varkw
 
defaults�
p 

 

kwonlyargs� 
kwonlydefaults
 
annotations� *
 z5trace_0z6trace_1
�
7trace_0
8trace_12�
E__inference_sequential_layer_call_and_return_conditional_losses_90996
E__inference_sequential_layer_call_and_return_conditional_losses_91011�
���
FullArgSpec)
args!�
jinputs

jtraining
jmask
varargs
 
varkw
 
defaults�
p 

 

kwonlyargs� 
kwonlydefaults
 
annotations� *
 z7trace_0z8trace_1
�B�
 __inference__wrapped_model_90954flatten_input"�
���
FullArgSpec
args� 
varargsjargs
varkwjkwargs
defaults
 

kwonlyargs� 
kwonlydefaults
 
annotations� *
 
�
9
_variables
:_iterations
;_learning_rate
<_index_dict
=	momentums
>_update_step_xla"
experimentalOptimizer
�B�
__inference_infer_90704x"�
���
FullArgSpec
args�
jx
varargs
 
varkw
 
defaults
 

kwonlyargs� 
kwonlydefaults
 
annotations� *!�
����������
�B�
__inference_restore_90723checkpoint_path"�
���
FullArgSpec
args�
jcheckpoint_path
varargs
 
varkw
 
defaults
 

kwonlyargs� 
kwonlydefaults
 
annotations� *�	
� 
�B�
__inference_save_90740checkpoint_path"�
���
FullArgSpec
args�
jcheckpoint_path
varargs
 
varkw
 
defaults
 

kwonlyargs� 
kwonlydefaults
 
annotations� *�	
� 
�B�
__inference_train_90935xy"�
���
FullArgSpec
args�

jx
jy
varargs
 
varkw
 
defaults
 

kwonlyargs� 
kwonlydefaults
 
annotations� *;�8
����������
����������

�B�
#__inference_signature_wrapper_90633xy"�
���
FullArgSpec
args� 
varargs
 
varkwjkwargs
defaults
 

kwonlyargs� 
kwonlydefaults
 
annotations� *
 
�B�
#__inference_signature_wrapper_90649x"�
���
FullArgSpec
args� 
varargs
 
varkwjkwargs
defaults
 

kwonlyargs� 
kwonlydefaults
 
annotations� *
 
�B�
#__inference_signature_wrapper_90663checkpoint_path"�
���
FullArgSpec
args� 
varargs
 
varkwjkwargs
defaults
 

kwonlyargs� 
kwonlydefaults
 
annotations� *
 
�B�
#__inference_signature_wrapper_90683checkpoint_path"�
���
FullArgSpec
args� 
varargs
 
varkwjkwargs
defaults
 

kwonlyargs� 
kwonlydefaults
 
annotations� *
 
 "
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_list_wrapper
�
?non_trainable_variables

@layers
Ametrics
Blayer_regularization_losses
Clayer_metrics
	variables
trainable_variables
regularization_losses
__call__
*&call_and_return_all_conditional_losses
&"call_and_return_conditional_losses"
_generic_user_object
�
Dtrace_02�
'__inference_flatten_layer_call_fn_91087�
���
FullArgSpec
args�

jinputs
varargs
 
varkw
 
defaults
 

kwonlyargs� 
kwonlydefaults
 
annotations� *
 zDtrace_0
�
Etrace_02�
B__inference_flatten_layer_call_and_return_conditional_losses_91093�
���
FullArgSpec
args�

jinputs
varargs
 
varkw
 
defaults
 

kwonlyargs� 
kwonlydefaults
 
annotations� *
 zEtrace_0
.
&0
'1"
trackable_list_wrapper
.
&0
'1"
trackable_list_wrapper
 "
trackable_list_wrapper
�
Fnon_trainable_variables

Glayers
Hmetrics
Ilayer_regularization_losses
Jlayer_metrics
 	variables
!trainable_variables
"regularization_losses
$__call__
*%&call_and_return_all_conditional_losses
&%"call_and_return_conditional_losses"
_generic_user_object
�
Ktrace_02�
'__inference_dense_1_layer_call_fn_91102�
���
FullArgSpec
args�

jinputs
varargs
 
varkw
 
defaults
 

kwonlyargs� 
kwonlydefaults
 
annotations� *
 zKtrace_0
�
Ltrace_02�
B__inference_dense_1_layer_call_and_return_conditional_losses_91113�
���
FullArgSpec
args�

jinputs
varargs
 
varkw
 
defaults
 

kwonlyargs� 
kwonlydefaults
 
annotations� *
 zLtrace_0
": 
��2dense_1/kernel
:�2dense_1/bias
.
.0
/1"
trackable_list_wrapper
.
.0
/1"
trackable_list_wrapper
 "
trackable_list_wrapper
�
Mnon_trainable_variables

Nlayers
Ometrics
Player_regularization_losses
Qlayer_metrics
(	variables
)trainable_variables
*regularization_losses
,__call__
*-&call_and_return_all_conditional_losses
&-"call_and_return_conditional_losses"
_generic_user_object
�
Rtrace_02�
'__inference_dense_2_layer_call_fn_91122�
���
FullArgSpec
args�

jinputs
varargs
 
varkw
 
defaults
 

kwonlyargs� 
kwonlydefaults
 
annotations� *
 zRtrace_0
�
Strace_02�
B__inference_dense_2_layer_call_and_return_conditional_losses_91132�
���
FullArgSpec
args�

jinputs
varargs
 
varkw
 
defaults
 

kwonlyargs� 
kwonlydefaults
 
annotations� *
 zStrace_0
!:	�
2dense_2/kernel
:
2dense_2/bias
 "
trackable_list_wrapper
5
0
1
	2"
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_dict_wrapper
�B�
*__inference_sequential_layer_call_fn_91024flatten_input"�
���
FullArgSpec)
args!�
jinputs

jtraining
jmask
varargs
 
varkw
 
defaults�
p 

 

kwonlyargs� 
kwonlydefaults
 
annotations� *
 
�B�
*__inference_sequential_layer_call_fn_91037flatten_input"�
���
FullArgSpec)
args!�
jinputs

jtraining
jmask
varargs
 
varkw
 
defaults�
p 

 

kwonlyargs� 
kwonlydefaults
 
annotations� *
 
�B�
E__inference_sequential_layer_call_and_return_conditional_losses_90996flatten_input"�
���
FullArgSpec)
args!�
jinputs

jtraining
jmask
varargs
 
varkw
 
defaults�
p 

 

kwonlyargs� 
kwonlydefaults
 
annotations� *
 
�B�
E__inference_sequential_layer_call_and_return_conditional_losses_91011flatten_input"�
���
FullArgSpec)
args!�
jinputs

jtraining
jmask
varargs
 
varkw
 
defaults�
p 

 

kwonlyargs� 
kwonlydefaults
 
annotations� *
 
C
:0
T1
U2
V3
W4"
trackable_list_wrapper
:	 2	iteration
: 2learning_rate
 "
trackable_dict_wrapper
<
T0
U1
V2
W3"
trackable_list_wrapper
�
Xtrace_0
Ytrace_1
Ztrace_2
[trace_32�
"__inference__update_step_xla_91067
"__inference__update_step_xla_91072
"__inference__update_step_xla_91077
"__inference__update_step_xla_91082�
���
FullArgSpec*
args"�

jgradient

jvariable
jkey
varargs
 
varkw
 
defaults
 

kwonlyargs� 
kwonlydefaults
 
annotations� *
 0zXtrace_0zYtrace_1zZtrace_2z[trace_3
 "
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_dict_wrapper
�B�
'__inference_flatten_layer_call_fn_91087inputs"�
���
FullArgSpec
args�

jinputs
varargs
 
varkw
 
defaults
 

kwonlyargs� 
kwonlydefaults
 
annotations� *
 
�B�
B__inference_flatten_layer_call_and_return_conditional_losses_91093inputs"�
���
FullArgSpec
args�

jinputs
varargs
 
varkw
 
defaults
 

kwonlyargs� 
kwonlydefaults
 
annotations� *
 
 "
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_dict_wrapper
�B�
'__inference_dense_1_layer_call_fn_91102inputs"�
���
FullArgSpec
args�

jinputs
varargs
 
varkw
 
defaults
 

kwonlyargs� 
kwonlydefaults
 
annotations� *
 
�B�
B__inference_dense_1_layer_call_and_return_conditional_losses_91113inputs"�
���
FullArgSpec
args�

jinputs
varargs
 
varkw
 
defaults
 

kwonlyargs� 
kwonlydefaults
 
annotations� *
 
 "
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_dict_wrapper
�B�
'__inference_dense_2_layer_call_fn_91122inputs"�
���
FullArgSpec
args�

jinputs
varargs
 
varkw
 
defaults
 

kwonlyargs� 
kwonlydefaults
 
annotations� *
 
�B�
B__inference_dense_2_layer_call_and_return_conditional_losses_91132inputs"�
���
FullArgSpec
args�

jinputs
varargs
 
varkw
 
defaults
 

kwonlyargs� 
kwonlydefaults
 
annotations� *
 
&:$
��2SGD/m/dense_1/kernel
:�2SGD/m/dense_1/bias
%:#	�
2SGD/m/dense_2/kernel
:
2SGD/m/dense_2/bias
�B�
"__inference__update_step_xla_91067gradientvariable"�
���
FullArgSpec*
args"�

jgradient

jvariable
jkey
varargs
 
varkw
 
defaults
 

kwonlyargs� 
kwonlydefaults
 
annotations� *
 
�B�
"__inference__update_step_xla_91072gradientvariable"�
���
FullArgSpec*
args"�

jgradient

jvariable
jkey
varargs
 
varkw
 
defaults
 

kwonlyargs� 
kwonlydefaults
 
annotations� *
 
�B�
"__inference__update_step_xla_91077gradientvariable"�
���
FullArgSpec*
args"�

jgradient

jvariable
jkey
varargs
 
varkw
 
defaults
 

kwonlyargs� 
kwonlydefaults
 
annotations� *
 
�B�
"__inference__update_step_xla_91082gradientvariable"�
���
FullArgSpec*
args"�

jgradient

jvariable
jkey
varargs
 
varkw
 
defaults
 

kwonlyargs� 
kwonlydefaults
 
annotations� *
 �
"__inference__update_step_xla_91067rl�i
b�_
�
gradient
��
6�3	�
�
��
�
p
` VariableSpec 
`������?
� "
 �
"__inference__update_step_xla_91072hb�_
X�U
�
gradient�
1�.	�
��
�
p
` VariableSpec 
`�ч���?
� "
 �
"__inference__update_step_xla_91077pj�g
`�]
�
gradient	�

5�2	�
�	�

�
p
` VariableSpec 
`��ҫ��?
� "
 �
"__inference__update_step_xla_91082f`�]
V�S
�
gradient

0�-	�
�

�
p
` VariableSpec 
`��ӫ��?
� "
 �
 __inference__wrapped_model_90954u&'./:�7
0�-
+�(
flatten_input���������
� "1�.
,
dense_2!�
dense_2���������
�
B__inference_dense_1_layer_call_and_return_conditional_losses_91113e&'0�-
&�#
!�
inputs����������
� "-�*
#� 
tensor_0����������
� �
'__inference_dense_1_layer_call_fn_91102Z&'0�-
&�#
!�
inputs����������
� ""�
unknown�����������
B__inference_dense_2_layer_call_and_return_conditional_losses_91132d./0�-
&�#
!�
inputs����������
� ",�)
"�
tensor_0���������

� �
'__inference_dense_2_layer_call_fn_91122Y./0�-
&�#
!�
inputs����������
� "!�
unknown���������
�
B__inference_flatten_layer_call_and_return_conditional_losses_91093d3�0
)�&
$�!
inputs���������
� "-�*
#� 
tensor_0����������
� �
'__inference_flatten_layer_call_fn_91087Y3�0
)�&
$�!
inputs���������
� ""�
unknown�����������
__inference_infer_90704�&'./.�+
$�!
�
x���������
� "[�X
*
logits �
logits���������

*
output �
output���������
�
"__inference_internal_grad_fn_91193����
���

 
!�
result_grads_0
��
�
result_grads_1�
 �
result_grads_2	�

�
result_grads_3

!�
result_grads_4
��
�
result_grads_5�
 �
result_grads_6	�

�
result_grads_7

� "{�x

 

 

 

 
�
tensor_4
��
�
tensor_5�
�
tensor_6	�

�
tensor_7
�
"__inference_internal_grad_fn_91220����
���

 
!�
result_grads_0
��
�
result_grads_1�
 �
result_grads_2	�

�
result_grads_3

!�
result_grads_4
��
�
result_grads_5�
 �
result_grads_6	�

�
result_grads_7

� "{�x

 

 

 

 
�
tensor_4
��
�
tensor_5�
�
tensor_6	�

�
tensor_7
�
__inference_restore_90723�&'./'�$
�
�
checkpoint_path 
� "���
+
dense_1/bias:0�
dense_1_bias_0
/
dense_1/kernel:0�
dense_1_kernel_0
+
dense_2/bias:0�
dense_2_bias_0
/
dense_2/kernel:0�
dense_2_kernel_0{
__inference_save_90740a&'./'�$
�
�
checkpoint_path 
� "0�-
+
checkpoint_path�
checkpoint_path �
E__inference_sequential_layer_call_and_return_conditional_losses_90996x&'./B�?
8�5
+�(
flatten_input���������
p

 
� ",�)
"�
tensor_0���������

� �
E__inference_sequential_layer_call_and_return_conditional_losses_91011x&'./B�?
8�5
+�(
flatten_input���������
p 

 
� ",�)
"�
tensor_0���������

� �
*__inference_sequential_layer_call_fn_91024m&'./B�?
8�5
+�(
flatten_input���������
p

 
� "!�
unknown���������
�
*__inference_sequential_layer_call_fn_91037m&'./B�?
8�5
+�(
flatten_input���������
p 

 
� "!�
unknown���������
�
#__inference_signature_wrapper_90633
&'./;TUVW:U�R
� 
K�H
$
x�
x���������
 
y�
y���������
"�

loss�

loss �
#__inference_signature_wrapper_90649�&'./3�0
� 
)�&
$
x�
x���������"[�X
*
logits �
logits���������

*
output �
output���������
�
#__inference_signature_wrapper_90663t&'./:�7
� 
0�-
+
checkpoint_path�
checkpoint_path "0�-
+
checkpoint_path�
checkpoint_path �
#__inference_signature_wrapper_90683�&'./:�7
� 
0�-
+
checkpoint_path�
checkpoint_path "���
+
dense_1/bias:0�
dense_1_bias_0
/
dense_1/kernel:0�
dense_1_kernel_0
+
dense_2/bias:0�
dense_2_bias_0
/
dense_2/kernel:0�
dense_2_kernel_0�
__inference_train_90935u
&'./;TUVW:K�H
A�>
�
x���������
�
y���������

� "�

loss�

loss 